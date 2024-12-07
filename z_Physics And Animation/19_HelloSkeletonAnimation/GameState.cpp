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

	mModelId = ModelManager::Get()->LoadModelId("../../Assets/Models/SillyDancing/SillyDancing.model");
	ModelManager::Get()->AddAnimation(mModelId, "../../Assets/Models/Animations/stepDance.animset");
	ModelManager::Get()->AddAnimation(mModelId, "../../Assets/Models/Animations/waveDance.animset");
	ModelManager::Get()->AddAnimation(mModelId, "../../Assets/Models/Animations/ymca.animset");
	mCharacter = CreateRenderGroup(mModelId, &mCharacterAnimator);
	mCharacterAnimator.Initialize(mModelId);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFilePath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	CleanupRenderGroup(mCharacter);
}

void GameState::Update(float deltaTime)
{
	mCharacterAnimator.Update(deltaTime);

#pragma region CameraMovement
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
	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(mModelId, boneTransforms, &mCharacterAnimator);
		AnimationUtil::DrawSkeleton(mModelId, boneTransforms);
	}

	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	if (!mDrawSkeleton)
	{
		mStandardEffect.Begin();
			DrawRenderGroup(mStandardEffect, mCharacter);
		mStandardEffect.End();
	}
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
		ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
		if (ImGui::DragInt("Animation", &mAnimIndex, 1, -1, mCharacterAnimator.GetAnimationCount() - 1))
		{
			mCharacterAnimator.PlayAnimation(mAnimIndex, true);
		}
		mStandardEffect.DebugUI();
	ImGui::End();
}
