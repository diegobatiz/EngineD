#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "InstanceVertexShader.h"
#include "BlendState.h"
#include "Sampler.h"
#include "TextureManager.h"

#include "Colours.h"

namespace EngineD::Graphics
{
	class Camera;

	class InstanceParticleEffect
	{
	public:
		void Initialize(std::filesystem::path snowTexturePath);
		void Terminate();

		void Update(float deltaTime);

		void Begin();
		void End();

		void DebugUI();
		void SetCamera(const Camera& camera);

	private:
		struct ParticleData
		{
			Math::Matrix4 matView;
			Math::Matrix4 matProj;
		};

		struct TimeData
		{
			float time;
			float padding[3];
		};

		using ParticleBuffer = TypedConstantBuffer<ParticleData>;
		ParticleBuffer mParticleBuffer;
		using TimeBuffer = TypedConstantBuffer<TimeData>;
		TimeBuffer mTimeBuffer;

		InstanceVertexShader mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		float mTime = 0.0f;
	};
}