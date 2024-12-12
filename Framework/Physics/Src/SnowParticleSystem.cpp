#include "Precompiled.h"
#include "SnowParticleSystem.h"

using namespace EngineD;
using namespace EngineD::Physics;
using namespace EngineD::Math;
using namespace EngineD::Graphics;

void SnowParticleSystem::Initialize(const SnowParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawnTime = 0;

	Mesh particleMesh = MeshBuilder::CreateSpriteQuad(0.5f, 0.5f);
	mParticleObject.meshBuffer.Initialize(particleMesh);
	mParticleObject.diffuseMapId = info.particleTextureId;

	InitializeParticles(info.maxParticles);
}

void SnowParticleSystem::Terminate()
{
	mParticleObject.Terminate();
	for (auto& p : mParticles)
	{
		p->Terminate();
		p.reset();
	}
}

void SnowParticleSystem::Update(float deltaTime)
{
	mNextSpawnTime -= deltaTime;
	if (mNextSpawnTime <= 0.0f)
	{
		SpawnParticles();
	}
	for (auto& p : mParticles)
	{
		p->Update(deltaTime);
	}
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
		ImGui::DragFloat("MinLife", &mInfo.minParticleLifetime, 0.25f);
		ImGui::DragFloat("MaxLife", &mInfo.maxParticleLifetime, 0.25f, mInfo.minParticleLifetime);
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

void SnowParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndices.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndices[i] = i;
		mParticles[i] = std::make_unique<Particle>();
		mParticles[i]->Initialize();
	}
}

void SnowParticleSystem::SpawnSingleParticle()
{
	Particle* particle = mParticles[mNextAvailableParticleIndex].get();
	mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();

	float randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posX = mInfo.spawnMin.x + ((mInfo.spawnMax.x - mInfo.spawnMin.x) * randFloat);

	randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posZ = mInfo.spawnMin.y + ((mInfo.spawnMax.y - mInfo.spawnMin.y) * randFloat);

	randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);

	ParticleActivateData activateData;

	float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	
	activateData.lifeTime = Lerp(mInfo.minParticleLifetime, mInfo.maxParticleLifetime, t);
	activateData.position = { posX, mInfo.spawnHeight, posZ };
	activateData.velocity = { 0, -1 * speed, 0 };
	particle->Activate(activateData);
}
