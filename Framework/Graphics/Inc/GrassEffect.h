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
	class FogEffect;

	class GrassEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Update(float deltaTime);
		void Begin();
		void End();
		void SetCamera(const Camera& camera);
		void SetGrassTextureID(TextureID id);
		void SetFogEffect(FogEffect& effect);
		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Vector3 cameraPos;
			float padding;
		};

		struct ColorData
		{
			EngineD::Color albedo1Colour;
			EngineD::Color albedo2Colour;
			EngineD::Color AOColour;
			EngineD::Color tipColour;
			EngineD::Color fogColour;
			float fogDensity;
			float fogOffset;
			float padding[2] = { 0.0f };
		};

		struct TimeData
		{
			float time;
			float padding[3] = { 0.0f };
		};

		struct GrassData
		{
			float extraHeightAdd = 2.0f;
			float extraHeightMult = 1.1f;
			float sway1 = 0.35f;
			float sway2 = 0.25f;
			float sway3 = 0.1f;
			float padding[3] = { 0.0f };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using ColorBuffer = TypedConstantBuffer<ColorData>;
		using TimeBuffer = TypedConstantBuffer<TimeData>;
		using GrassBuffer = TypedConstantBuffer<GrassData>;

		TransformBuffer mTransformBuffer;
		ColorBuffer mColorBuffer;
		TimeBuffer mTimeBuffer;
		GrassBuffer mGrassBuffer;

		Sampler mSampler;
		GrassVertexShader mVertexShader;
		PixelShader_D3D11 mPixelShader;

		TextureID mGrassTextureId;

		const Camera* mCamera = nullptr;
		FogEffect* mFogEffect;
		ColorData mColorData;
		float mCurrentTime;
		GrassData mGrassData;
	};
}