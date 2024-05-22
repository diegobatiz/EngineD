#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "RenderTarget.h"

namespace EngineD::Graphics
{
	struct RenderObject;

	class GaussianBlurEffect final
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetSourceTexture(const Texture& texture);
		const Texture& GetHorizontalBlurTexture() const;
		const Texture& GetVerticalBlurTexture() const;
		const Texture& GetResultTexture() const;

	private:
		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;
		};

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		RenderTarget mHorizontalBlurRenderTexture;
		RenderTarget mVerticalBlurRenderTexture;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mHorizontalBlurPS;
		PixelShader_D3D11 mVerticalBlurPS;

		Sampler mSampler;
		
		const Texture* mSourceTexture = nullptr;
		int mBlurIterations = 1;
		float mBlurSaturation = 1.0f;
	};
}