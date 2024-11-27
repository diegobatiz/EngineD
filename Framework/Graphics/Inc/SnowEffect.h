#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"

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
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;

		TransformBuffer mTransformBuffer;

		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		const Texture* mPositionMap = nullptr;
	};
}