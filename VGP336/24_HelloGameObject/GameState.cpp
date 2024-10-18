#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;
using namespace EngineD::Physics;

void GameState::Initialize()
{
	mGameWorld.Initialize();

	GameObject* gameObject = mGameWorld.CreateGameObject("Transform");

	gameObject->AddComponent<TransformComponent>();
	gameObject->Initialize();

	GameObject* cameraGameObject = mGameWorld.CreateGameObject("Camera");

	CameraComponent* camera = cameraGameObject->AddComponent<CameraComponent>();
	cameraGameObject->AddComponent<FPSCameraComponent>();
	cameraGameObject->Initialize();
	camera->GetCamera().SetPosition({ 0.0f, 2.0f, -2.0 });
	camera->GetCamera().SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Terminate();
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