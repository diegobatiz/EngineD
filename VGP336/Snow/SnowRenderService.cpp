#include "SnowRenderService.h"


using namespace EngineD;
using namespace EngineD::Graphics;

void SnowRenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	MeshPX quad = MeshBuilder::CreateScreenQuad();
	mQuad.meshBuffer.Initialize(quad);

	mPlayerPositionEffect.Initialize();

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
	mSnowEffect.SetCamera(mCameraService->GetMain());

	mPlayerPositionEffect.Begin();
		mPlayerPositionEffect.Render(mQuad);
	mPlayerPositionEffect.End();

	mSnowEffect.Begin();
		mSnowEffect.Render(mSnow);
	mSnowEffect.End();
}

void SnowRenderService::DebugUI()
{
	if (ImGui::CollapsingHeader("Rendering"))
	{
		ImGui::Text("FPS: %f", mFPS);
		if (ImGui::CollapsingHeader("Light"))
		{
			if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
			{
				mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
		}
		mSnowEffect.DebugUI();
		mPlayerPositionEffect.DebugUI();
	}
}

void SnowRenderService::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
}

void SnowRenderService::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("SnowSize"))
	{
		const float snowSize = value["SnowSize"].GetFloat();

		mPlayerPositionEffect.SetSnowDimensions(snowSize, snowSize);

		MeshD mesh = MeshBuilder::CreatePlane(snowSize * 10, snowSize * 10, 0.1f, Colors::White);
		mSnow.meshBuffer.Initialize(mesh);
	}
}

void SnowRenderService::RegisterPlayer(const TransformComponent* player)
{
	ASSERT(mPlayerTransform == nullptr, "SnowRenderService: player component already registered!");
	mPlayerTransform = player->GetTransform();

	mPlayerPositionEffect.SetPlayerTransform(*mPlayerTransform);

	const MeshComponent* meshComponent = player->GetOwner().GetComponent<MeshComponent>();
	mPlayerPositionEffect.SetRadius(meshComponent->GetSize());
}
