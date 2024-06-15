#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "RenderTarget.h"

namespace EngineD::Graphics
{
	struct RenderObject;

	class ComicBookEffect final
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetNormalTexture(const Texture& texture);
		void SetDepthTexture(const Texture& texture);
		const Texture& GetResultTexture() const;

	private:
		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float padding[2];
		};

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		RenderTarget mSobelRenderTexture;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		Sampler mSampler;

		const Texture* mNormalTexture = nullptr;
		const Texture* mDepthTexture = nullptr;
		int mDrawIterations = 1;
	};
}