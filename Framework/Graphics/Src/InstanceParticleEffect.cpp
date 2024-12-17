#include "Precompiled.h"
#include "InstanceParticleEffect.h"

#include "Camera.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"
#include "TextureManager.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Math;

void InstanceParticleEffect::Initialize(std::filesystem::path snowTexturePath)
{
	std::filesystem::path filePath = L"../../Assets/Shaders/SnowParticle.fx";
	mVertexShader.Initialize<VertexP>(filePath);
	mPixelShader.Initialize(filePath);
	mParticleBuffer.Initialize();
	mTimeBuffer.Initialize();
}

void InstanceParticleEffect::Terminate()
{
	mTimeBuffer.Terminate();
	mParticleBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void InstanceParticleEffect::Update(float deltaTime)
{
	mTime += deltaTime;
}

void InstanceParticleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mParticleBuffer.BindVS(0);

	mTimeBuffer.BindVS(1);

	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	ParticleData data;
	data.matProj = matProj;
	data.matView = matView;

	TimeData time;
	time.time = mTime;

	mParticleBuffer.Update(data);
	mTimeBuffer.Update(time);
}

void InstanceParticleEffect::End()
{
}


void InstanceParticleEffect::DebugUI()
{
	//
}

void InstanceParticleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
