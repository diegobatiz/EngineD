#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	CreateShape();

	//GraphicsSystem::Get()->CreateTriangles(mVertices);

	//std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoSomething.fx";

	//GraphicsSystem::Get()->CreateShaders(shaderFilePath);
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
	//GraphicsSystem::Get()->Render(mVertices.size());
}

void GameState::CreateShape()
{

}

void TriangleState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::D))
	{
		MainApp().ChangeState("TriforceState");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::A))
	{
		MainApp().ChangeState("HeartState");
	}
}

void TriangleState::CreateShape()
{
	//mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	//mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Green });
	//mVertices.push_back({ { 0.5f, 0.0f,  0.0f }, Colors::Blue });
}

void TriforceState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::D))
	{
		MainApp().ChangeState("DiamondState");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::A))
	{
		MainApp().ChangeState("TriangleState");
	}
}

void TriforceState::CreateShape()
{
	//mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::Yellow });

	//mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { 0.5f, -0.5f, 0.0f }, Colors::Yellow });

	//mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { 0.0f, 0.5f, 0.0f }, Colors::Yellow });
	//mVertices.push_back({ { 0.25f, 0.0f, 0.0f }, Colors::Yellow });
}

void DiamondState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::D))
	{
		MainApp().ChangeState("HeartState");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::A))
	{
		MainApp().ChangeState("TriforceState");
	}
}

void DiamondState::CreateShape()
{
	/*mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Blue });
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
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::AntiqueWhite });*/
}

void HeartState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::A))
	{
		MainApp().ChangeState("DiamondState");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::D))
	{
		MainApp().ChangeState("TriangleState");
	}
}

void HeartState::CreateShape()
{/*
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
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Pink });*/
}
