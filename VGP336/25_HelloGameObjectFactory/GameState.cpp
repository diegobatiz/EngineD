#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;
using namespace EngineD::Physics;

void GameState::Initialize()
{
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize();

	mGameWorld.CreateGameObject("Object0", "../../Assets/Templates/testObject.json");
	mGameWorld.CreateGameObject("Camera", "../../Assets/Templates/fps_camera.json");
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