#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "HullShader.h"

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
		void SetPositionMap(const Texture& posMap) { mPositionMap = &posMap; }

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Vector3 cameraPos;
		};

		struct TessellationData
		{
			float minTessDistance = 10;
			float maxTessDistance = 20;
			float tessLevel = 4;
			float padding;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using TessellationBuffer = TypedConstantBuffer<TessellationData>;

		TransformBuffer mTransformBuffer;
		TessellationBuffer mTessBuffer;

		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		HullShader mHullShader;

		const Camera* mCamera = nullptr;
		const Texture* mPositionMap = nullptr;
		TessellationData mTessData;
	};
}