#include "Precompiled.h"
#include "GameWorld.h"

using namespace EngineD;

void GameWorld::Initialize()
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");

	mInitialized = true;
}

void GameWorld::Terminate()
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->Terminate();
		gameObject.reset();
	}
	mGameObjects.clear();
}

void GameWorld::Update(float deltaTime)
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void GameWorld::Render()
{
}

void GameWorld::DebugUI()
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->DebugUI();
	}
}

GameObject* GameWorld::CreateGameObject(std::string name)
{
	auto& newGameObject = mGameObjects.emplace_back(std::make_unique<GameObject>());
	newGameObject->SetName(name);
	return newGameObject.get();
}
