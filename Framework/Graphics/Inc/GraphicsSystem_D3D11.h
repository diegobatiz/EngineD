#pragma once

#include "Colours.h"
#include "GraphicsSystem.h"

namespace EngineD::Graphics
{
	class Graphics_D3D11 final : GraphicsSystem
	{
	public:
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static Graphics_D3D11* Get();

		Graphics_D3D11() = default;
		virtual ~Graphics_D3D11();

		Graphics_D3D11(const Graphics_D3D11&) = delete;
		Graphics_D3D11(const Graphics_D3D11&&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void EndRender();

		void ToggleFullScreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTarget();
		void ResetViewport();

		void SetClearColour(const Colour& colour);
		void SetVSync(bool vSync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;

		float GetBackBufferAspectRatio() const;

		ID3D11Device* GetDevice() { return mD3DDevice; }
		ID3D11DeviceContext* GetContext() { return mImmediateContext; }

	private:
		ID3D11Device* mD3DDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;

		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;

		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;

		DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
		D3D11_VIEWPORT mViewPort{};

		Colour mClearColour = Colours::Black;
		UINT mVSync = 1;
	};
}