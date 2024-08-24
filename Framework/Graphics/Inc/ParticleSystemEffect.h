#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "BlendState.h"
#include "Sampler.h"

#include "Colours.h"

namespace EngineD::Graphics
{
	class Camera;
	struct RenderObject;

	class ParticleSystemEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderObject& renderObject, const Color& color);

		void DebugUI();
		void SetCamera(const Camera& camera);

	private:
		struct ParticleData
		{
			Math::Matrix4 wvp;
			Color color;
		};

		using ParticleBuffer = TypedConstantBuffer<ParticleData>;
		ParticleBuffer mParticleBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		Sampler mSampler;
		BlendState mBlendState;

		const Camera* mCamera = nullptr;
	};
}