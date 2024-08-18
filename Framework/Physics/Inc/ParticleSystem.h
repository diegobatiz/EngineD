#pragma once

#include "Particle.h"

namespace EngineD::Physics
{
	struct ParticleSystemInfo
	{
		int maxParticles = 100;
		Graphics::TextureID particleTextureId = 0;
		Math::Vector3 spawnPosition = Math::Vector3::Zero;
		Math::Vector3 spawnDirection = Math::Vector3::YAxis;
		float spawnDelay = 0.0f;
		float spawnLifeTime = 0.0f;
		int minParticlePerEmit = 0;
		int maxParticlePerEmit = 0;
		float minTimeBetweenEmit = 0.0f;
		float maxTimeBetweenEmit = 0.0f;
		float minSpawnAngle = 0.0f;
		float maxSpawnAngle = 0.0f;
		float minSpeed = 0.0f;
		float maxSpeed = 0.0f;
		float minParticleLifetime = 0.0f;
		float maxParticleLifetime = 0.0f;
		Color minStartColor = Colors::White;
		Color maxStartColor = Colors::White;
		Color minEndColor = Colors::White;
		Color maxEndColor = Colors::White;
		Math::Vector3 minStartScale = Math::Vector3::One;
		Math::Vector3 maxStartScale = Math::Vector3::One;
		Math::Vector3 minEndScale = Math::Vector3::One;
		Math::Vector3 maxEndScale = Math::Vector3::One;
	};

	class ParticleSystem
	{
	public:
		void Initialize(const ParticleSystemInfo& info);
		void Terminate();
		void Update(float deltaTime);

		bool IsActive() const;
		void DebugUI();

		void SetPosition(const Math::Vector3& position);
		void SetCamera(const Graphics::Camera& camera);

		void SpawnParticles();

		template<class Effect>
		void Render(Effect& effect)
		{
			if (IsActive())
			{
				CurrentParticleInfo info;
				for (const int& index : mParticleIndices)
				{
					Particle* particle = mParticle[index].get();
					if (particle->IsActive())
					{
						particle->ObtainCurrentInfo(info);
						mParticleObject.transform = info.transform;
						effect.Render(mParticleObject, info.color);
					}
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
		const Graphics::Camera* mCamera = nullptr;

		//Particle System Info
		ParticleSystemInfo mInfo;
		int mNextAvailableParticleIndex;
		float mNextSpawnTime = 0.0f;
		float mLifeTime = 0.0f;
	};
}