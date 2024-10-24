#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;
using namespace EngineD::Physics;

void GameState::Initialize()
{
	mGameWorld.AddService<CameraService>();
	mGameWorld.Initialize();

	GameObject* gameObject = mGameWorld.CreateGameObject("Transform", "../../Assets/Templates/test_object.json");

	GameObject* cameraGameObject = mGameWorld.CreateGameObject("Camera", "../../Assets/Templates/fps_camera.json");

	CameraComponent* camera = cameraGameObject->GetComponent<CameraComponent>();
	camera->GetCamera().SetPosition({ 0.0f, 2.0f, -2.0 });
	camera->GetCamera().SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		mGameWorld.DebugUI();
	ImGui::End();
}