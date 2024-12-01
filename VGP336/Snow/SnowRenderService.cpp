#include "SnowRenderService.h"


using namespace EngineD;
using namespace EngineD::Graphics;

void SnowRenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	MeshD mesh = MeshBuilder::CreatePlane(100, 100, 0.1f, Colors::White);
	mSnow.meshBuffer.Initialize(mesh);

	MeshPX quad = MeshBuilder::CreateScreenQuad();
	mQuad.meshBuffer.Initialize(quad);

	mPlayerPositionEffect.Initialize();

	mSnowEffect.SetCamera(mCameraService->GetMain());
	mSnowEffect.SetPositionMap(mPlayerPositionEffect.GetPositionMap());
	mSnowEffect.Initialize();
}

void SnowRenderService::Terminate()
{
	mSnowEffect.Terminate();
	mPlayerPositionEffect.Terminate();
	mQuad.Terminate();
	mSnow.Terminate();
}

void SnowRenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void SnowRenderService::Render()
{
	mPlayerPositionEffect.Begin();
		mPlayerPositionEffect.Render(mQuad);
	mPlayerPositionEffect.End();

	mSnowEffect.Begin();
		mSnowEffect.Render(mSnow);
	mSnowEffect.End();
}

void SnowRenderService::DebugUI()
{

}

void SnowRenderService::RegisterPlayer(const TransformComponent* player)
{
	ASSERT(mPlayerTransform == nullptr, "SnowRenderService: player component already registered!");
	mPlayerTransform = player;

	mPlayerPositionEffect.SetPlayerTransform(mPlayerTransform->GetWorldTransform());
}

void SnowRenderService::UnegisterPlayer(const TransformComponent* player)
{
}

void SnowRenderService::Register(const RenderObjectComponent* roc)
{
}

void SnowRenderService::Unregister(const RenderObjectComponent* roc)
{
}
