#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "RenderTarget.h"

namespace EngineD::Graphics
{
	struct RenderObject;

	class AtmosphereEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetCamera(const Camera& camera);

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

		const Camera* mCamera;
	};
}