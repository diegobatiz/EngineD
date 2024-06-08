#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	TextureManager* tm = TextureManager::Get();
	circle1 = MeshBuilder::CreateSphere(300, 300, 3.0f);
	circle2 = MeshBuilder::CreateSphere(300, 300, 2.0f);
	circle3 = MeshBuilder::CreateSphere(300, 300, 1.0f);
	horizontalPlane = MeshBuilder::CreateHorizontalPlane(10, 10, 10);
	verticalPlane = MeshBuilder::CreateHorizontalPlane(10, 10, 10);

	circleObj1.meshBuffer.Initialize(circle1);

	circleObj2.meshBuffer.Initialize(circle2);
	circleObj3.meshBuffer.Initialize(circle3);
	hPlane.meshBuffer.Initialize(horizontalPlane);
	vPlane.meshBuffer.Initialize(verticalPlane);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/CelEffect.fx";
	mStandardEffect.Initialize(shaderFilePath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	circleObj1.Terminate();
	circleObj2.Terminate();
	circleObj3.Terminate();
	hPlane.Terminate();
	vPlane.Terminate();
}

void GameState::Update(float deltaTime)
{
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
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
		mStandardEffect.Render(circleObj1);
		mStandardEffect.Render(circleObj2);
		mStandardEffect.Render(circleObj3);
		mStandardEffect.Render(hPlane);
		mStandardEffect.Render(vPlane);
	mStandardEffect.End();
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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);

		ImGui::DragFloat("SpecPower##Material", &mRenderObject.material.power, 1.0f, 1.0f, 100.0f);
	}
		mStandardEffect.DebugUI();
	ImGui::End();
}
