#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Colours.h"
#include "TextureManager.h"

namespace EngineD::Graphics
{
	class Camera;
	class Texture;
	struct RenderObject;

	class SnowEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera) { mCamera = &camera; }
		void SetPositionMap(const Texture& posMap);

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Vector3 cameraPos;
			float padding;
		};

		struct TessellationData
		{
			float minTessDistance = 5;
			float maxTessDistance = 20;
			float tessLevel = 16;
			float bumpOffset = 0.1f;
		};

		struct LightingSettings
		{
			Math::Vector4 bottomColor = {0.639215f, 0.639215f, 0.639215f, 1.0f};
			Math::Vector4 topColor = Colors::White;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using TessellationBuffer = TypedConstantBuffer<TessellationData>;
		using LightingBuffer = TypedConstantBuffer<LightingSettings>;

		TransformBuffer mTransformBuffer;
		TessellationBuffer mTessBuffer;
		LightingBuffer mLightingBuffer;

		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		HullShader mHullShader;
		DomainShader mDomainShader;

		const Camera* mCamera = nullptr;
		const Texture* mPositionMap = nullptr;
		TextureID mSnowBumpMapID;
		TextureID mSnowTextureID;
		TessellationData mTessData;
		LightingSettings mLightingSettings;
	};
}