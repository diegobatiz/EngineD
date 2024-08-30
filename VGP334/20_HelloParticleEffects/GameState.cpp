#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Physics;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	ParticleSystemInfo info;
	info.maxParticles = 100;
	info.particleTextureId = TextureManager::Get()->LoadTexture("Images/pikachu.png");
	info.spawnPosition = Math::Vector3::Zero;
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.spawnLifeTime = 9999999999999.0f;
	info.minParticlePerEmit = 1;
	info.maxParticlePerEmit = 3;
	info.minTimeBetweenEmit = 0.01f;
	info.maxTimeBetweenEmit = 0.05f;
	info.minSpawnAngle = -30.0f * Math::Constants::Pi / 180.0f;
	info.maxSpawnAngle = 30.0f * Math::Constants::Pi / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minParticleLifetime = 0.5f;
	info.maxParticleLifetime = 3.0f;
	info.minStartColor = Colors::Red;
	info.maxStartColor = Colors::Red;
	info.minEndColor = Colors::Yellow;
	info.maxEndColor = Colors::Yellow;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = { 1.5f, 1.5f, 1.5f };
	info.minEndScale = { 0.5f, 0.5f, 0.5f };
	info.maxEndScale = { 0.1f, 0.1f, 0.1f };
	mParticleSystem.Initialize(info);
	mParticleSystem.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mParticleSystem.Terminate();
	mParticleEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
#pragma region Camera Stuff
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
#pragma endregion

	mParticleSystem.Update(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mParticleEffect.Begin();
		mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		Physics::PhysicsWorld::Get()->DebugUI();
		mParticleEffect.DebugUI();
		mParticleSystem.DebugUI();
	ImGui::End();

	SimpleDraw::Render(mCamera);
}
