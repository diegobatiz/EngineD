#pragma once
#include "ConstantBuffer.h"
#include "Sampler.h"
#include "GrassShader.h"
#include "PixelShader_D3D11.h"

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
		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;

		TransformBuffer mTransformBuffer;

		Sampler mSampler;
		GrassVertexShader mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
	};
}