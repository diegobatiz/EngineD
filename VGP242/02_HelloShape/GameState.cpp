#include "GameState.h"

using namespace EngineD::Graphics;

void GameState::Initialize()
{
	LOG("GAME STATE INITIALIZED");
	GraphicsSystem::Get()->SetClearColour(Colours::Blue);
	mLifeTime = 2.0f;
}

void GameState::Terminate()
{
	LOG("GAME STATE TERMINATED");
}

void GameState::Update(float deltaTime)
{
	mLifeTime -= deltaTime;
	if (mLifeTime <= 0.0f)
	{
		App& myApp = EngineD::MainApp();
		myApp.ChangeState("MainState");
	}
}