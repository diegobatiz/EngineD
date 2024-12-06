#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "HullShader.h"
#include "DomainShader.h"

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
			float tessLevel = 8;
			float padding;
		};

		struct LightingSettings
		{
			float textureSize = 1024;
			float normalStrength = 0.7f;
			float padding[2];
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
		TessellationData mTessData;
		LightingSettings mLightingSettings;
	};
}