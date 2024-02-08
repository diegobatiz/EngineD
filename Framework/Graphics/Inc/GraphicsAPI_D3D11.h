#pragma once

#include "GraphicsAPI.h"

namespace EngineD::Graphics
{
	class Graphics_D3D11 final : public GraphicsAPI
	{
	public:
		Graphics_D3D11() = default;
		virtual ~Graphics_D3D11();

		Graphics_D3D11(const Graphics_D3D11&) = delete;
		Graphics_D3D11(const Graphics_D3D11&&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void Render();
		void EndRender();

		void ToggleFullScreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTarget();
		void ResetViewport();

		void SetClearColor(const Color& color); 
		void SetTopology(Topology topology);

		template<class VertexType>
		void CreateTriangles(const std::vector<VertexType>& vertices);
		void CreateShaders(std::filesystem::path filePath);

	private:
		void SetVSync(bool vSync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;

		float GetBackBufferAspectRatio() const;

		//MeshBuffer mMeshBuffer
		//VertexShader mVertexShader
		// PixelShader mPixelShader
		// 
		// when create triangles is call -> mesh buffer is initialized
		// when create pixel/vertex shader is called -> shaders initialized
		// 
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;

		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		uint32_t mVertexSize;
		uint32_t mVertexCount;
		uint32_t mIndexCount;

		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
		ID3D11PixelShader* mPixelShader = nullptr;
		

		ID3D11Device* mD3DDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;

		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;

		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;

		DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
		D3D11_VIEWPORT mViewPort{};

		Color mClearColor = Colors::Black;
		UINT mVSync = 1;
	};
}