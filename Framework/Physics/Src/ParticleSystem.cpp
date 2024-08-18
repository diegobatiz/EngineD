#include "Precompiled.h"
#include "ParticleSystem.h"

using namespace EngineD;
using namespace EngineD::Physics;
using namespace EngineD::Math;
using namespace EngineD::Graphics;

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawnTime = info.spawnDelay;
	mLifeTime = info.spawnLifeTime;

	Mesh particleMesh = MeshBuilder::CreateSpriteQuad(0.5f, 0.5f);
	mParticleObject.meshBuffer.Initialize(particleMesh);
	mParticleObject.diffuseMapId = info.particleTextureId;

	InitializeParticles(info.maxParticles);
}

void ParticleSystem::InitializeParticles(uint32_t count)
{

}

void ParticleSystem::Terminate()
{
	mParticleObject.Terminate();
	for (auto& p : mParticles)
	{
		p->Terminate();
		p.reset();
	}
}

void ParticleSystem::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		mNextSpawnTime -= deltaTime;
		if (mNextSpawnTime <= 0.0f && mLifeTime > 0.0f)
		{
			SpawnParticles();
		}
		for (auto& p : mParticles)
		{
			p->Update(deltaTime);
		}
		std::sort(mParticleIndices.begin(), mParticleIndices.end(), [&](const int& a, const int& b))
	}
}

bool ParticleSystem::IsActive() const
{
	return false;
}

void ParticleSystem::DebugUI()
{	 
}	 
	 
void ParticleSystem::SetPosition(const Math::Vector3& position)
{	 
}	 
	 
void ParticleSystem::SetCamera(const Graphics::Camera& camera)
{	 
}	 
	 
void ParticleSystem::SpawnParticles()
{	 
}	  
	 
void ParticleSystem::SpawnSingleParticle()
{
}
