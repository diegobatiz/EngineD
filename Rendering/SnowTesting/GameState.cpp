#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

#define PI = 3.14f

void GameState::Initialize()
{
	m_Camera.SetPosition({ -1.0f, 4.0f, -8.0f });
	m_Camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	m_DirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });

	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mTransform.position = { 0.0f, 0.0f, 0.0f };

	MeshD mesh = MeshBuilder::CreatePlane(100, 100, 0.1f, Colors::White);
	mSnow.meshBuffer.Initialize(mesh);

	MeshPX quad = MeshBuilder::CreateScreenQuad();
	mQuad.meshBuffer.Initialize(quad);

	mPositionMapEffect.SetPlayerTransform(mTransform);
	mPositionMapEffect.SetSnowDimensions(10, 10);
	mPositionMapEffect.SetRadius(0.5f);
	mPositionMapEffect.Initialize();

    mSnowEffect.SetCamera(m_Camera);
	mSnowEffect.SetPositionMap(mPositionMapEffect.GetPositionMap());
	mSnowEffect.Initialize();
}

void GameState::Terminate()
{
	mPositionMapEffect.Terminate();
	mSnow.Terminate();
}

void GameState::Update(float deltaTime)
{
#pragma region Camera Movement
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		m_Camera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		m_Camera.Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		m_Camera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		m_Camera.Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		m_Camera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		m_Camera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		m_Camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		m_Camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
#pragma endregion

}

void GameState::Render()
{
	mPositionMapEffect.Begin();
		mPositionMapEffect.Render(mQuad);
	mPositionMapEffect.End();

	mSnowEffect.Begin();
		mSnowEffect.Render(mSnow);
	mSnowEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Math::Vector3 position = m_Camera.GetPosition();
		ImGui::DragFloat3("Camera Position", &position.x, 0.1f);
		if (ImGui::DragFloat3("Directional Light", &m_DirectionalLight.direction.x, 0.01f))
		{
			m_DirectionalLight.direction = m_DirectionalLight.direction;
		}

		mPositionMapEffect.DebugUI();

	ImGui::End();
}
