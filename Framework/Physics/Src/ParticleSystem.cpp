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
	mParticleIndices.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndices[i] = i;
		mParticles[i] = std::make_unique<Particle>();
		mParticles[i]->Initialize();
	}
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
		std::sort(mParticleIndices.begin(), mParticleIndices.end(), [&](const int& a, const int& b)
			{
				float distSqrA = MagnitudeSqr(mParticles[a]->GetPosition() - mCamera->GetPosition());
				float distSqrB = MagnitudeSqr(mParticles[b]->GetPosition() - mCamera->GetPosition());
				return distSqrB < distSqrA;
			});
	}
}

bool ParticleSystem::IsActive() const
{
	if (mLifeTime > 0.0f)
	{
		return true;
	}

	for (auto& p : mParticles)
	{
		if (p->IsActive())
		{
			return true;
		}
	}

	return false;
}

void ParticleSystem::DebugUI()
{	 
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::DragFloat3("Position", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("Direction", &mInfo.spawnDirection.x))
		{
			mInfo.spawnDirection = Normalize(mInfo.spawnDirection);
		}
		ImGui::DragInt("MinPerEmit", &mInfo.minParticlePerEmit);
		ImGui::DragInt("MaxPerEmit", &mInfo.maxParticlePerEmit, 1, mInfo.minParticlePerEmit);
		ImGui::DragFloat("MinTimeToEmit", &mInfo.minTimeBetweenEmit);
		ImGui::DragFloat("MaxTimeToEmit", &mInfo.maxTimeBetweenEmit, mInfo.minTimeBetweenEmit);
		ImGui::DragFloat("MinSpawnAngle", &mInfo.minSpawnAngle, 0.001f, -Math::Constants::Pi);
		ImGui::DragFloat("MaxSpawnAngle", &mInfo.maxSpawnAngle, 0.001f, mInfo.minSpawnAngle);
		ImGui::DragFloat("MinSpeed", &mInfo.minSpeed);
		ImGui::DragFloat("MaxSpeed", &mInfo.maxSpeed,1.0f, mInfo.minSpeed);
		ImGui::DragFloat("MinLife", &mInfo.minParticleLifetime, 0.25f);
		ImGui::DragFloat("MaxLife", &mInfo.maxParticleLifetime, 0.25f, mInfo.minParticleLifetime);
		ImGui::ColorEdit4("MinStartColor", &mInfo.minStartColor.r);
		ImGui::ColorEdit4("MaxStartColor", &mInfo.maxStartColor.r);
		ImGui::ColorEdit4("MinStartColor", &mInfo.minEndColor.r);
		ImGui::ColorEdit4("MinStartColor", &mInfo.maxEndColor.r);
		ImGui::DragFloat3("MinStartScale", &mInfo.minStartScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MaxStartScale", &mInfo.maxStartScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MinEndScale", &mInfo.minEndScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MaxEndScale", &mInfo.minEndScale.x, 0.1f, 0.001f);
	}
}	 
	 
void ParticleSystem::SetPosition(const Math::Vector3& position)
{	 
	mInfo.spawnPosition = position;
}	 
	 
void ParticleSystem::SetCamera(const Graphics::Camera& camera)
{	 
	mCamera = &camera;
}	 
	 
void ParticleSystem::SpawnParticles()
{	 
	int numParticles = mInfo.maxParticlePerEmit + (rand() & (mInfo.maxParticlePerEmit - mInfo.minParticlePerEmit + 1));
	for (int i = 0; i < numParticles; i++)
	{
		SpawnSingleParticle();
	}
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	mNextSpawnTime = mInfo.minTimeBetweenEmit + ((mInfo.maxTimeBetweenEmit - mInfo.minTimeBetweenEmit) * randFloat);
}	  
	 
void ParticleSystem::SpawnSingleParticle()
{
	Particle* particle = mParticles[mNextAvailableParticleIndex].get();
	mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();
	
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randAngle = mInfo.minSpawnAngle + ((mInfo.maxSpawnAngle - mInfo.minSpawnAngle) * randFloat);

	Vector3 rotAxis = (Abs(Dot(mInfo.spawnDirection, Vector3::YAxis)) >= 0.9f) ? Vector3::XAxis : Vector3::YAxis;
	Matrix4 matRotA = Matrix4::RotationAxis(rotAxis, randAngle);

	rotAxis = Normalize(Cross(rotAxis, mInfo.spawnDirection));

	Matrix4 matRot = Matrix4::RotationAxis(rotAxis, randAngle) * matRotA;
	Vector3 spawnDirection = TransformNormal(mInfo.spawnDirection, matRot);

	randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);

	ParticleActivateData activateData;

	float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.startColor = Lerp(mInfo.minStartColor, mInfo.maxStartColor, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.endColor = Lerp(mInfo.minEndColor, mInfo.maxEndColor, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.startScale = Lerp(mInfo.minStartScale, mInfo.maxStartScale, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.endScale = Lerp(mInfo.minEndScale, mInfo.maxEndScale, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.lifeTime = Lerp(mInfo.minParticleLifetime, mInfo.maxParticleLifetime, t);
	activateData.position = mInfo.spawnPosition;
	activateData.velocity = spawnDirection * speed;
	particle->Activate(activateData);
}
