#include "Precompiled.h"
#include "GraphicsSystem_D3D11.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Core;

namespace
{
	std::unique_ptr<Graphics_D3D11> sGraphicsSystem;
	WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK Graphics_D3D11::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem != nullptr)
	{
		switch (message)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicsSystem->Resize(width, height);
				break;
			}
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void Graphics_D3D11::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "GraphicsSystem: is already initialized");
	sGraphicsSystem = std::make_unique<Graphics_D3D11>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void Graphics_D3D11::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		sGraphicsSystem.reset();
	}
}

Graphics_D3D11* Graphics_D3D11::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem: is not initialized");
	return sGraphicsSystem.get();
}

Graphics_D3D11::~Graphics_D3D11()
{
	ASSERT(mD3DDevice == nullptr, "GraphicsSystem: must be terminated");
}

void Graphics_D3D11::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect{};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		nullptr,
		&mImmediateContext
	);

	ASSERT(SUCCEEDED(hr), "GraphicsSYstem: failed to initialize device or swap chain");
	mSwapChain->GetDesc(&mSwapChainDesc);

	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}

void Graphics_D3D11::Terminate()
{
	sWindowMessageHandler.Unhook();

	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3DDevice);
}

void Graphics_D3D11::BeginRender()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColour));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void Graphics_D3D11::EndRender()
{
	mSwapChain->Present(mVSync, 0);
}

void Graphics_D3D11::ToggleFullScreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void Graphics_D3D11::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to access swap chain view");

		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to get back buffer");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to create render target");

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = GetBackBufferWidth();
	descDepth.Height = GetBackBufferHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to create stencil buffer");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to create depth stencil view");

	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mViewPort.Width = static_cast<float>(GetBackBufferWidth());
	mViewPort.Height = static_cast<float>(GetBackBufferHeight());
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
	mImmediateContext->RSSetViewports(1, &mViewPort);
}

void Graphics_D3D11::ResetRenderTarget()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void Graphics_D3D11::ResetViewport()
{
	mImmediateContext->RSSetViewports(1, &mViewPort);
}

void Graphics_D3D11::SetClearColour(const Colour& colour)
{
	mClearColour = colour;
}

void Graphics_D3D11::SetVSync(bool vSync)
{
	mVSync = vSync ? 1 : 0;
}

uint32_t Graphics_D3D11::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t Graphics_D3D11::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}

float Graphics_D3D11::GetBackBufferAspectRatio() const
{
	return static_cast<float>(GetBackBufferWidth())/static_cast<float>(GetBackBufferHeight());
}