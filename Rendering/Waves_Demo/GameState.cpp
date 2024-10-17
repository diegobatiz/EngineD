#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	m_Camera.SetPosition({ -1.0f, 4.0f, -8.0f });
	m_Camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	GraphicsSystem::Get()->SetClearColor(Colors::SkyBlue);

	MeshD mesh = MeshBuilder::CreatePlane(10, 10, 1.0f, Colors::DarkBlue);
	m_Water.meshBuffer.Initialize(mesh);

	//Vector2 direction;
	//Vector2 origin;       --not needed right now
	//float   frequency;
	//float   amplitude;
	//float   phase;
	//float   steepness;

	WaveData wave =
	{
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	mWaves.push_back(wave);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/WaveShader.fx";
	m_WaveEffect.InitializeWaves(mWaves);
	m_WaveEffect.Initialize(shaderFilePath);
	m_WaveEffect.SetCamera(m_Camera);
}

void GameState::Terminate()
{
	m_WaveEffect.Terminate();
	m_Water.Terminate();
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

	m_WaveEffect.Update(deltaTime);
}

void GameState::Render()
{
	m_WaveEffect.Begin();
		m_WaveEffect.Render(m_Water);
	m_WaveEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Math::Vector3 position = m_Camera.GetPosition();
		ImGui::DragFloat3("Camera Position", &position.x, 0.1f);

		m_WaveEffect.DebugUI();

	ImGui::End();
}
