#pragma once
#include "ConstantBuffer.h"
#include "Sampler.h"
#include "GrassShader.h"
#include "PixelShader_D3D11.h"
#include "TextureManager.h"
#include "BlendState.h"
#include "Colours.h"

namespace EngineD::Graphics
{
	class Camera;

	class GrassEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();
		void SetCamera(const Camera& camera);
		void SetGrassTextureID(TextureID id);
		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		struct ColorData
		{
			EngineD::Color albedo1Colour;
			EngineD::Color albedo2Colour;
			EngineD::Color AOColour;
			EngineD::Color tipColour;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using ColorBuffer = TypedConstantBuffer<ColorData>;

		TransformBuffer mTransformBuffer;
		ColorBuffer mColorBuffer;

		Sampler mSampler;
		GrassVertexShader mVertexShader;
		PixelShader_D3D11 mPixelShader;
		BlendState mBlendState;

		TextureID mGrassTextureId;

		const Camera* mCamera = nullptr;
		ColorData mColorData;
	};
}