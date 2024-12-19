#include "Precompiled.h"
#include "SnowParticleSystem.h"

#include "MeshBuilder.h"]
#include "Noise.h"

using namespace EngineD;
using namespace EngineD::Math;
using namespace EngineD::Graphics;

void SnowParticleSystem::Initialize(const SnowParticleSystemInfo& info)
{
	mInfo = info;
	mNextSpawnTime = 0;

	MeshP particleMesh = MeshBuilder::CreateSpriteQuadP(info.particlesSize, info.particlesSize);
	mParticleObject.meshBuffer.Initialize(particleMesh);

	mParticleMeshBuffer.Initialize<MeshP>(particleMesh, info.maxParticles);

	InitializeParticles(info.maxParticles);
}

void SnowParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndices.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		float randNumX = std::rand() % 100;
		float randNumY = std::rand() % 100;
		mParticleIndices[i] = i;
		mParticles[i].noise.x = Noise::NoiseSimplex({ randNumX + i * mNoiseFrequency, randNumY * mNoiseFrequency });
		mParticles[i].noise.y = Noise::NoiseSimplex({ randNumX * mNoiseFrequency, randNumY - i * mNoiseFrequency });
		mParticles[i].noise *= mNoiseAmplitude;
	}
}

void SnowParticleSystem::Terminate()
{
	mParticleObject.Terminate();
}

void SnowParticleSystem::Update(float deltaTime)
{
	if (mPause)
	{
		return;
	}
	mNextSpawnTime -= deltaTime;
	if (mNextSpawnTime <= 0.0f)
	{
		SpawnParticles();
	}
	for (auto& p : mParticles)
	{
		if (!p.alive) continue;

		p.position.y += p.speed * deltaTime;
		if (p.position.y < -2)
		{
			RemoveSingleParticle(&p);
		}
	}
}


void SnowParticleSystem::Render()
{
	mParticleMeshBuffer.UpdateInstanceBuffer(mParticles);

	mParticleMeshBuffer.Render();
}

void SnowParticleSystem::DebugUI()
{
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::DragInt("MinPerEmit", &mInfo.minParticlePerEmit);
		ImGui::DragInt("MaxPerEmit", &mInfo.maxParticlePerEmit, 1, mInfo.minParticlePerEmit);
		ImGui::DragFloat("MinTimeToEmit", &mInfo.minTimeBetweenEmit, 0.01f);
		ImGui::DragFloat("MaxTimeToEmit", &mInfo.maxTimeBetweenEmit, 0.01f, mInfo.minTimeBetweenEmit);
		ImGui::DragFloat("MinSpeed", &mInfo.minSpeed);
		ImGui::DragFloat("MaxSpeed", &mInfo.maxSpeed, 1.0f, mInfo.minSpeed);
	
		if (ImGui::Button("Pause Particles"))
		{
			mPause = !mPause;
		}
	}
}

void SnowParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.maxParticlePerEmit + (rand() & (mInfo.maxParticlePerEmit - mInfo.minParticlePerEmit + 1));
	for (int i = 0; i < numParticles; i++)
	{
		SpawnSingleParticle();
	}
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	mNextSpawnTime = mInfo.minTimeBetweenEmit + ((mInfo.maxTimeBetweenEmit - mInfo.minTimeBetweenEmit) * randFloat);
}

void SnowParticleSystem::SpawnSingleParticle()
{
	if (mParticleIndices.empty())
	{
		return;
	}
	size_t index = mParticleIndices.back();
	mParticleIndices.pop_back();
	SnowParticle* particle = &mParticles[index];

	float randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posX = mInfo.spawnMin.x + ((mInfo.spawnMax.x - mInfo.spawnMin.x) * randFloat);

	randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posZ = mInfo.spawnMin.y + ((mInfo.spawnMax.y - mInfo.spawnMin.y) * randFloat) + (mInfo.spawnMax.y / 2);

	randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);
	
	particle->position = { posX, mInfo.spawnHeight, posZ };
	particle->speed = speed;
	particle->alive = true;
}

void SnowParticleSystem::RemoveSingleParticle(SnowParticle* p)
{
	size_t index = p - &mParticles[0];
	p->alive = false;
	mParticleIndices.push_back(index);
}
