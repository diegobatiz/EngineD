#pragma once

#include "GraphicsType.h"
#include "MeshBuffer_D3D11.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "ConstantBuffer.h"

namespace EngineD::Graphics
{
	class Graphics_D3D11 final : public GraphicsType
	{
	public:
		Graphics_D3D11() = default;
		virtual ~Graphics_D3D11();

		Graphics_D3D11(const Graphics_D3D11&) = delete;
		Graphics_D3D11(const Graphics_D3D11&&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&) = delete;
		Graphics_D3D11& operator=(const Graphics_D3D11&&) = delete;

		void Initialize(HWND window, bool fullscreen) override;
		void Terminate() override;

		void BeginRender() override;
		void Render() override;
		void EndRender() override;

		void ToggleFullScreen() override;
		void Resize(uint32_t width, uint32_t height) override;

		void ResetRenderTarget() override;
		void ResetViewport() override;

		void SetClearColor(const Color& color) override;

		uint32_t GetBackBufferWidth() const override;
		uint32_t GetBackBufferHeight() const override;

		float GetBackBufferAspectRatio() const override;

		template<class VertexType>
		void CreateMeshBuffer(const std::vector<VertexType>& vertices)
		{
			mMeshBuffer.InitDevice(mD3DDevice);
			mMeshBuffer.Initialize(vertices);
		}
		template<class MeshType>
		void CreateMeshBuffer(const MeshType& mesh)
		{
			mMeshBuffer.InitDevice(mD3DDevice);
			mMeshBuffer.Initialize(mesh);
		}
		
		template<class VertexType>
		void CreateVertexShader(std::filesystem::path filePath)
		{
			mVertexShader.InitDevice(mD3DDevice);
			mVertexShader.Initialize<VertexType>(filePath);
		}
		void CreatePixelShader(std::filesystem::path filePath)
		{
			mPixelShader.InitDevice(mD3DDevice);
			mPixelShader.Initialize(filePath);
		}

		void InitializeBuffer(uint32_t bufferSize);
		void UpdateBuffer(const void* data) const;

		ID3D11Device* GetDevice() { return mD3DDevice; }
		ID3D11DeviceContext* GetContext() { return mImmediateContext; }

	private:
		void SetVSync(bool vSync);

		MeshBuffer_D3D11 mMeshBuffer; 
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		ConstantBuffer mConstantBuffer;

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