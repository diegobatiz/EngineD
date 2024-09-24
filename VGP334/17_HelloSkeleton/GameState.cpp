#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	GraphicsSystem::Get()->SetClearColor(Colors::SkyBlue);

	Mesh mesh;
	const float hw = 1.0f * 0.5f;
	const float hh = 1.0f * 0.5f;
	const float qSqr2 = sqrt(2.0f) * 0.25f;

	mesh.vertices.push_back({ {-hw, -hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-hw,  hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 0.0f} });
	mesh.vertices.push_back({ { hw,  hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 0.0f} });
	mesh.vertices.push_back({ { hw, -hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 1.0f} });

	mesh.vertices.push_back({ {-qSqr2, -hh, qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-qSqr2,  hh, qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 0.0f} });
	mesh.vertices.push_back({ { qSqr2,  hh, -qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 0.0f} });
	mesh.vertices.push_back({ { qSqr2, -hh, -qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 1.0f} });

	mesh.vertices.push_back({ {-qSqr2, -hh, -qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-qSqr2,  hh, -qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 0.0f} });
	mesh.vertices.push_back({ {qSqr2,  hh, qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 0.0f} });
	mesh.vertices.push_back({ {qSqr2, -hh, qSqr2}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 1.0f} });

	

	mesh.indices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11
	};

	mGrassBuffer.SetInstanceAmount(100);
	mGrassBuffer.Initialize(mesh);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/GrassShader.fx";
	mGrassEffect.Initialize(shaderFilePath);
	mGrassEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mGrassEffect.Terminate();
	mGrassBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
#pragma region Camera Movement
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
}

void GameState::Render()
{
	mGrassEffect.Begin();
		mGrassBuffer.Render();
	mGrassEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
			{
				mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
		}
	ImGui::End();
}
