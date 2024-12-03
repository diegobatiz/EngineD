#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;
using namespace EngineD::Physics;

#include "CustomPlayerControllerComponent.h"
#include "SnowRenderService.h"

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "CustomPlayerControllerComponent")
	{
		return gameObject.AddComponent<CustomPlayerControllerComponent>();
	}
	return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "CustomPlayerControllerComponent")
	{
		return gameObject.GetComponent<CustomPlayerControllerComponent>();
	}
	return nullptr;
}

Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
	if (serviceName == "SnowRenderService")
	{
		return gameWorld.AddService<SnowRenderService>();
	}
	return nullptr;
}

void GameState::Initialize()
{
	GameObjectFactory::SetCustomGet(CustomComponentGet);
	GameObjectFactory::SetCustomMake(CustomComponentMake);
	GameWorld::SetCustomService(CustomServiceMake);
	mGameWorld.LoadLevel(L"../../Assets/Templates/Level/SnowLevel.json");
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