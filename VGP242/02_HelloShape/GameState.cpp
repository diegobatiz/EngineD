#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void GameState::Initialize()
{
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colours::Red });
	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colours::Red });
	mVertices.push_back({ { 0.5f, 0.0f,  0.0f }, Colours::Red });

	GraphicsSystem::Get()->CreateTriangles(mVertices);

	std::filesystem::path shaderFilePath = L"C:/Dev/Quarter 10/EngineD/Assets/Shaders/DoSomething.fx";

	GraphicsSystem::Get()->CreateShaders(shaderFilePath);
}

void GameState::Terminate()
{
	mVertices.clear();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	GraphicsSystem::Get()->Render(3);
}
