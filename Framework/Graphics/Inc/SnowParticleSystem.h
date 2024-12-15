#pragma once

#include "DynamicInstancedMeshBuffer.h"
#include "Camera.h"
#include "RenderObject.h"

namespace EngineD::Graphics
{
	struct SnowParticleSystemInfo
	{
		int maxParticles = 1000;
		float spawnHeight = 0;
		Math::Vector2 spawnMin = Math::Vector2::Zero;
		Math::Vector2 spawnMax = Math::Vector2::One;
		int minParticlePerEmit = 0;
		int maxParticlePerEmit = 0;
		float minTimeBetweenEmit = 0.0f;
		float maxTimeBetweenEmit = 0.0f;
		float minSpeed = 0.0f;
		float maxSpeed = 0.0f;
		float particlesSize;
	};

	struct SnowParticle
	{
		Math::Vector3 position = Math::Vector3::Zero;

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

		void SetCamera(const Camera& camera) { mCamera = &camera; }

	private:
		void InitializeParticles(uint32_t count);
		void SpawnSingleParticle();
		void RemoveSingleParticle(SnowParticle* particle);

		DynamicInstancedMeshBuffer mParticleMeshBuffer;

		//Particles
		using SnowParticles = std::vector<SnowParticle>;
		SnowParticles mParticles;
		std::vector<int> mParticleIndices;

		//Particle System Info
		SnowParticleSystemInfo mInfo;
		float mNextSpawnTime = 0.0f;

		//Render Info
		RenderObject mParticleObject;

		const Camera* mCamera;
		bool mPause = false;
	};
}