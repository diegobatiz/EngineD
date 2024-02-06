#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void GameState::Initialize()
{
	CreateShape();

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
	GraphicsSystem::Get()->Render(mVertices.size());
}

void GameState::CreateShape()
{

}

void TriangleState::CreateShape()
{
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Green });
	mVertices.push_back({ { 0.5f, 0.0f,  0.0f }, Colors::Blue });
}

void TriforceState::CreateShape()
{
	mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::Yellow });

	mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.5f, -0.5f, 0.0f }, Colors::Yellow });

	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.0f, 0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Yellow });
}

void DiamondState::CreateShape()
{
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::AntiqueWhite });

	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::AntiqueWhite });
	mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::AntiqueWhite });
	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::AntiqueWhite });
}

void HeartState::CreateShape()
{
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::DeepPink });
	mVertices.push_back({ { -0.25f, 0.25f, 0.0f }, Colors::HotPink });

	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.25f, 0.25f, 0.0f }, Colors::HotPink });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::DeepPink });

	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::DeepPink });
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Pink });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::DeepPink });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Pink });
}
