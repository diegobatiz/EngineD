#pragma once

#include "Particle.h"

namespace EngineD::Physics
{
	struct SnowParticleSystemInfo
	{
		int maxParticles = 1000;
		Graphics::TextureID particleTextureId = 0;
		float spawnHeight = 0;
		Math::Vector2 spawnMin = Math::Vector2::Zero;
		Math::Vector2 spawnMax = Math::Vector2::One;
		int minParticlePerEmit = 0;
		int maxParticlePerEmit = 0;
		float minTimeBetweenEmit = 0.0f;
		float maxTimeBetweenEmit = 0.0f;
		float minSpeed = 0.0f;
		float maxSpeed = 0.0f;
		float minParticleLifetime = 0.0f;
		float maxParticleLifetime = 0.0f;
	};

	class SnowParticleSystem
	{
	public:
		void Initialize(const SnowParticleSystemInfo& info);
		void Terminate();
		void Update(float deltaTime);

		void DebugUI();

		void SpawnParticles();

		template<class Effect>
		void Render(Effect& effect)
		{
			CurrentParticleInfo info;
			for (const int& index : mParticleIndices)
			{
				Particle* particle = mParticles[index].get();
				if (particle->IsActive())
				{
					particle->ObtainCurrentInfo(info);
					mParticleObject.transform = info.transform;
					effect.Render(mParticleObject, info.color);
				}
			}
		}

	private:
		void InitializeParticles(uint32_t count);
		void SpawnSingleParticle();

		//Particles
		using Particles = std::vector<std::unique_ptr<Particle>>;
		Particles mParticles;
		std::vector<int> mParticleIndices;

		//Render Info
		Graphics::RenderObject mParticleObject;

		//Particle System Info
		SnowParticleSystemInfo mInfo;
		int mNextAvailableParticleIndex;
		float mNextSpawnTime = 0.0f;
	};
}