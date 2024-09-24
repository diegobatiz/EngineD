#include "Precompiled.h"
#include "GrassEffect.h"

#include "Camera.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void GrassEffect::Initialize(const std::filesystem::path& filename)
{
	mTransformBuffer.Initialize();
	mVertexShader.Initialize(filename);
	mPixelShader.Initialize(filename);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GrassEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
}

void GrassEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);

	mTransformBuffer.Update(data);
}

void GrassEffect::End()
{
}

void GrassEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void GrassEffect::DebugUI()
{
}
