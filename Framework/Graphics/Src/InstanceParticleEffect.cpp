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
	mVertexShader.Initialize(filePath);
	mPixelShader.Initialize(filePath);
	mParticleBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
	mTextureId = TextureManager::Get()->LoadTexture(snowTexturePath);
}

void InstanceParticleEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();
	mParticleBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void InstanceParticleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mBlendState.Set();

	mSampler.BindPS(0);

	mParticleBuffer.BindVS(0);

	Math::Matrix4 matWorld = Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	ParticleData data;
	data.wvp = Transpose(matFinal);

	mParticleBuffer.Update(data);

	TextureManager::Get()->BindPS(mTextureId, 0);
}

void InstanceParticleEffect::End()
{
	mBlendState.ClearState();
}


void InstanceParticleEffect::DebugUI()
{
	//
}

void InstanceParticleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
