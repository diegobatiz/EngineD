#include "Precompiled.h"
#include "SnowParticleSystem.h"

#include "MeshBuilder.h"

using namespace EngineD;
using namespace EngineD::Math;
using namespace EngineD::Graphics;

void SnowParticleSystem::Initialize(const SnowParticleSystemInfo& info)
{
	mInfo = info;
	mNextSpawnTime = 0;

	Mesh particleMesh = MeshBuilder::CreateSpriteQuad(0.5f, 0.5f);
	mParticleObject.meshBuffer.Initialize(particleMesh);

	mParticleMeshBuffer.Initialize<Mesh>(particleMesh, info.maxParticles);

	InitializeParticles(info.maxParticles);
}

void SnowParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndices.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndices[i] = i;
	}
}

void SnowParticleSystem::Terminate()
{
	mParticleObject.Terminate();
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
		if (!p.alive) continue;

		p.position.y += p.speed * 0.1;
		if (p.position.y < -2)
		{
			RemoveSingleParticle(&p);
		}
	}
}


void SnowParticleSystem::Render()
{
	// Sort Particles
	Math::Vector3 cameraPosition = mCamera->GetPosition();
	std::sort(mParticles.begin(), mParticles.end(),
		[&cameraPosition](const SnowParticle& A, const SnowParticle& B)
		{
			float distA = (A.position.x - cameraPosition.x) * (A.position.x - cameraPosition.x) +
				(A.position.y - cameraPosition.y) * (A.position.y - cameraPosition.y) +
				(A.position.z - cameraPosition.z) * (A.position.z - cameraPosition.z);
			float distB = (B.position.x - cameraPosition.x) * (B.position.x - cameraPosition.x) +
				(B.position.y - cameraPosition.y) * (B.position.y - cameraPosition.y) +
				(B.position.z - cameraPosition.z) * (B.position.z - cameraPosition.z);
			return distA > distB;
		});

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

	LOG("Particle Spawned %d", (int)index);

	float randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posX = mInfo.spawnMin.x + ((mInfo.spawnMax.x - mInfo.spawnMin.x) * randFloat);

	randFloat = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1;
	float posZ = mInfo.spawnMin.y + ((mInfo.spawnMax.y - mInfo.spawnMin.y) * randFloat);

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

	LOG("Particle Removed %d", (int)index);
}
