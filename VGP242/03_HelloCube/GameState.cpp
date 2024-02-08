#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mMesh.vertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });
	mMesh.vertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::DeepPink });
	mMesh.vertices.push_back({ { -0.25f, 0.25f, 0.0f }, Colors::HotPink });

	mMesh.vertices.push_back({ { 0.25f, 0.25f, 0.0f }, Colors::HotPink });
	mMesh.vertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::DeepPink });

	mMesh.vertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Pink });

	mMesh.indices =
	{
		0, 1, 2,
		0, 3, 4,
		4, 5, 0,
		1, 0, 5
	};

	GraphicsSystem::Get()->CreateTriangles(mMesh.vertices);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoSomething.fx";

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
	GraphicsSystem::Get()->Render(mVertices.size(), mIndices.size());
}