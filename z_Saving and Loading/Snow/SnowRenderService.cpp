#include "SnowRenderService.h"

#include "CustomPlayerControllerComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void SnowRenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	MeshPX quad = MeshBuilder::CreateScreenQuad();
	mQuad.meshBuffer.Initialize(quad);

	mPlayerPositionEffect.Initialize();

	mSnowEffect.Initialize();

	//mTemplateFilePath = GetWorld().GetLevelPath();
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

	bool moving = mPlayerController->IsMoving();
	mPlayerPositionEffect.SetPlayerMoving(moving);
	mPlayerPositionEffect.Update(deltaTime);
}

void SnowRenderService::Render()
{
	mSnowEffect.SetCamera(mCameraService->GetMain());
	mSnowEffect.SetPositionMap(mPlayerPositionEffect.GetPositionMap());

	mPlayerPositionEffect.Begin();
		mPlayerPositionEffect.Render(mQuad);
	mPlayerPositionEffect.End();

	mSnowEffect.Begin();
		mSnowEffect.Render(mSnow);
	mSnowEffect.End();
}

void SnowRenderService::DebugUI()
{
	if (ImGui::CollapsingHeader("Snow Rendering"))
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

void SnowRenderService::Save()
{
	//if (mTemplateFilePath.empty())
	{
		return;
	}

	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value components(rapidjson::kObjectType);
	Serialize(doc, components);
	doc.AddMember("Components", components, doc.GetAllocator());

	FILE* file = nullptr;
	//auto err = fopen_s(&file, mTemplateFilePath.c_str(), "w");
	//ASSERT(err == 0, "GameObject: failed to open template file %s", mTemplateFilePath.c_str());

	char writeBuffer[655536];
	rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	doc.Accept(writer);
	fclose(file);
}

void SnowRenderService::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("SnowSize"))
	{
		const float snowSize = value["SnowSize"].GetFloat();

		mPlayerPositionEffect.SetSnowDimensions(snowSize, snowSize);

		MeshD mesh = MeshBuilder::CreatePlane(snowSize * 2, snowSize * 2, 0.5f, Colors::White);
		mSnow.meshBuffer.Initialize(mesh);
		mSnow.meshBuffer.SetTopology(MeshBuffer::Topology::TriangleControlPoint);
	}
}

void SnowRenderService::RegisterPlayer(const TransformComponent* player)
{
	ASSERT(mPlayerTransform == nullptr, "SnowRenderService: player component already registered!");
	mPlayerTransform = player->GetTransform();
	mPlayerController = player->GetOwner().GetComponent<CustomPlayerControllerComponent>();

	mPlayerPositionEffect.SetPlayerTransform(*mPlayerTransform);

	const MeshComponent* meshComponent = player->GetOwner().GetComponent<MeshComponent>();
	mPlayerPositionEffect.SetRadius(meshComponent->GetSize());
}
