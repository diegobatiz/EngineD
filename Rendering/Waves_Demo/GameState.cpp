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

	mWaveLoader.Initialize();
	mWaveLoader.SetEffect(&m_WaveEffect);

	GraphicsSystem::Get()->SetClearColor(Colors::SkyBlue);

	MeshPC mesh = MeshBuilder::CreateHorizontalPlanePC(2000, 2000, 0.1f, Colors::Blue);
	m_Water.meshBuffer.Initialize(mesh);

	MeshPX sky = MeshBuilder::CreateSkyBoxPX(600);
	mSky.meshBuffer.Initialize(sky);
	mSky.diffuseMapId = TextureManager::Get()->LoadTexture("skybox/skybox_texture.jpg");

	mAtmosphereEffect.Initialize();
	mAtmosphereEffect.SetCamera(m_Camera);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/WaveShader.fx";
	m_WaveEffect.Initialize(shaderFilePath);
	m_WaveEffect.SetCamera(m_Camera);
	m_WaveEffect.SetWaveData(mWaveLoader.GetData(), mWaveLoader.GetLightData());
	m_WaveEffect.SetDirectionalLight(m_DirectionalLight);
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
	//mAtmosphereEffect.Begin();
	//	mAtmosphereEffect.Render(mSky);
	//mAtmosphereEffect.End();

	m_WaveEffect.Begin();
		m_WaveEffect.Render(m_Water);
	m_WaveEffect.End();
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

		if (ImGui::ColorEdit3("Sky Color", &mColor.r))
		{
			GraphicsSystem::Get()->SetClearColor(mColor);
		}

		m_WaveEffect.DebugUI();

		mWaveLoader.DebugUI();

	ImGui::End();
}
