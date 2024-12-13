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
	};

	struct SnowParticle
	{
		Math::Vector3 position;

		float speed = -100.0f;
		bool alive;
	};

	class SnowParticleSystem
	{
	public:
		void Initialize(const SnowParticleSystemInfo& info);
		void Terminate();
		void Update(float deltaTime);

		void DebugUI();

		void SpawnParticles();

		void Render();

		void SetCamera(const EngineD::Graphics::Camera& camera) { mCamera = &camera; }

	private:
		void InitializeParticles(uint32_t count);
		void SpawnSingleParticle();
		void RemoveSingleParticle(SnowParticle* particle);

		EngineD::Graphics::DynamicInstancedMeshBuffer mParticleMeshBuffer;

		//Particles
		using SnowParticles = std::vector<SnowParticle>;
		SnowParticles mParticles;
		std::vector<int> mParticleIndices;

		//Particle System Info
		SnowParticleSystemInfo mInfo;
		float mNextSpawnTime = 0.0f;

		//Render Info
		Graphics::RenderObject mParticleObject;

		const EngineD::Graphics::Camera* mCamera;
	};
}