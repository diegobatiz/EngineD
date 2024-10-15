#include "Precompiled.h"
#include "WaveEffect.h"

#include "VertexTypes.h"
#include "Camera.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void WaveEffect::Initialize(const std::filesystem::path& filename)
{
	mTimeBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<VertexD>(filename);
	mPixelShader.Initialize(filename);
}

void WaveEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mTimeBuffer.Terminate();
}

void WaveEffect::Update(float deltaTime)
{
	mCurrentTime += deltaTime;
}

void WaveEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mTimeBuffer.BindVS(1);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	Math::Vector3 camPos = mCamera->GetPosition();

	TransformData data;
	data.wvp = Math::Transpose(matFinal);

	mTransformBuffer.Update(data);

	TimeData timeData;
	timeData.time = mCurrentTime;

	mTimeBuffer.Update(timeData);
}

void WaveEffect::End()
{
}

void WaveEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void WaveEffect::DebugUI()
{
}
