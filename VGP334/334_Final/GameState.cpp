#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0, 2.5f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	Mesh sky = MeshBuilder::CreateSkySphere(60, 60, 100.0f);
	mSky.meshBuffer.Initialize(sky);
	mSky.diffuseMapId = TextureManager::Get()->LoadTexture("skysphere/CloudySky.jpg");
	mSky.UseLighting(false);

	mModelIdA = ModelManager::Get()->LoadModelId("../../Assets/Models/Racer/Racer.model");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Walking.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Idle.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Waving.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Arguing.animset");
	mCharacterA = CreateRenderGroup(mModelIdA, &mCharacterAnimatorA);
	mCharacterAnimatorA.Initialize(mModelIdA);

	mModelIdB = ModelManager::Get()->LoadModelId("../../Assets/Models/Joe/Joe.model");
	ModelManager::Get()->AddAnimation(mModelIdB, "../../Assets/Models/Joe/JoeAnimations/Waving.animset");
	ModelManager::Get()->AddAnimation(mModelIdB, "../../Assets/Models/Joe/JoeAnimations/Arguing.animset");
	ModelManager::Get()->AddAnimation(mModelIdB, "../../Assets/Models/Joe/JoeAnimations/Death.animset");
	ModelManager::Get()->AddAnimation(mModelIdB, "../../Assets/Models/Joe/JoeAnimations/DeathLastFrame.animset");
	ModelManager::Get()->AddAnimation(mModelIdB, "../../Assets/Models/Joe/JoeAnimations/Idle.animset");
	mCharacterB = CreateRenderGroup(mModelIdB, &mCharacterAnimatorB);
	mCharacterAnimatorB.Initialize(mModelIdB);

	mModelIdC = ModelManager::Get()->LoadModelId("../../Assets/Models/Guy/Guy.model");
	ModelManager::Get()->AddAnimation(mModelIdC, "../../Assets/Models/Guy/GuyAnimations/Run.animset");
	ModelManager::Get()->AddAnimation(mModelIdC, "../../Assets/Models/Guy/GuyAnimations/Attack.animset");
	mCharacterC = CreateRenderGroup(mModelIdC, &mCharacterAnimatorC);
	mCharacterAnimatorC.Initialize(mModelIdC);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFilePath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mAnimationTime = 0.0f;
	mAnimationA = AnimationBuilder()
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ 0.0f, 0.0f, -10.0f }, 6.0f)
		.AddRotationKey({ 0, 0, 0, 0 }, 7.0f)
		.AddPositionKey({ 0.0f, 0.0f, -12.0f }, 7.0f)
		.AddRotationKey({ 0, -0.7071, 0, 0.7071 }, 7.1f)
		.AddPositionKey({ 0.0f, 0.0f, -12.0f }, 100.0f)
		.Build();

	mAnimationB = AnimationBuilder()
		.AddPositionKey({ 4.0f, 0.0f, -12.0f }, 0.0f)
		.AddRotationKey({ 0, 0.7071f, 0, 0.7071f }, 0.0f)
		.AddPositionKey({ 4.0f, 0.0f, -12.0f }, 16.48f)
		.AddPositionKey({ 2.5f, 0.0f, -12.0f }, 16.5f)
		.AddPositionKey({ 2.5f, 0.0f, -12.0f }, 100.0f)
		.Build();

	mAnimationC = AnimationBuilder()
		.AddPositionKey({ 2.5f, 0.0f, -30.0f }, 0.0f)
		.AddRotationKey({ 0, 1, 0, 0 }, 0.0f)
		.AddPositionKey({ 2.5f, 0.0f, -40.0f }, 26.0f)
		.AddPositionKey({ 2.5f, 0.0f, -13.3f }, 36.0f)
		.Build();

	mCameraAnimation = AnimationBuilder()
		.AddPositionKey({ 0, 2.5f, -3.0f }, 0.0f)
		.AddPositionKey({ 0, 2.5f, -13.0f }, 6.09f)
		.AddPositionKey({ 2.5f, 1.7f, -12.5f }, 6.1f)
		.AddPositionKey({ 2.5f, 1.7f, -12.5f }, 8.99f)
		.AddPositionKey({ 1.15f, 1.6f, -12.3f }, 9.0f)
		.AddPositionKey({ 1.15f, 1.6f, -12.3f }, 9.99f)
		.AddPositionKey({ 2.5f, 1.7f, -12.5f }, 10.0f)
		.AddPositionKey({ 2.5f, 1.7f, -12.5f }, 11.99f)
		.AddPositionKey({ 1.15f, 1.6f, -12.3f }, 12.0f)
		.AddPositionKey({ 1.15f, 1.6f, -12.3f }, 16.49f)
		.AddPositionKey({ 1.6f, 2.3f, -14.6f }, 16.5f)
		.AddPositionKey({ 1.6f, 2.3f, -14.6f }, 25.99f)
		.AddPositionKey({ 1.0f, 2.3f, -9.7f }, 26.0f)
		.AddPositionKey({ 1.0f, 2.3f, -9.7f }, 34.99f)
		.AddPositionKey({ 0.325f, 2.0f, -11.0f }, 35.0f)
		.AddPositionKey({ 0.325f, 2.0f, -11.0f }, 38.05f)
		.AddPositionKey({ 4.1f, 1.13f, -12.92f }, 38.06f)
		.Build();

	mEvents = AnimationBuilder()
		.AddEventKey(std::bind(&GameState::SetCameraLookAtB, this), 6.1f)
		.AddEventKey(std::bind(&GameState::IdleAnimationA, this), 7.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtA, this), 9.01f)
		.AddEventKey(std::bind(&GameState::FaceLightingLeft, this), 9.0f)
		.AddEventKey(std::bind(&GameState::FaceLightingRight, this), 10.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtB, this), 10.01f)
		.AddEventKey(std::bind(&GameState::FaceLightingLeft, this), 12.0f)
		.AddEventKey(std::bind(&GameState::WavingAnimationA, this), 12.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtA, this), 12.01f)
		.AddEventKey(std::bind(&GameState::FaceLightingMiddle, this), 16.5f)
		.AddEventKey(std::bind(&GameState::ArgueAnimationA, this), 16.5f)
		.AddEventKey(std::bind(&GameState::ArgueAnimationB, this), 16.5f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtMiddle, this), 16.51f)
		.AddEventKey(std::bind(&GameState::FaceLightingBack, this), 26.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtC, this), 26.01f)
		.AddEventKey(std::bind(&GameState::FaceLightingRight, this), 35.0f)
		.AddEventKey(std::bind(&GameState::AttackAnimationC, this), 35.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtB, this), 35.01f)
		.AddEventKey(std::bind(&GameState::DeathAnimationB, this), 35.9f)
		.AddEventKey(std::bind(&GameState::DeathLastFrameB, this), 38.06f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtDeath, this), 38.07f)
		.AddEventKey(std::bind(&GameState::FaceLightingMiddle, this), 38.06f)
		.Build();

	ChangeAnimation(1, mCharacterAnimatorA);
	ChangeAnimation(1, mCharacterAnimatorB);
	ChangeAnimation(1, mCharacterAnimatorC);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	CleanupRenderGroup(mCharacterC);
	CleanupRenderGroup(mCharacterB);
	CleanupRenderGroup(mCharacterA);
	mSky.Terminate();
}

void GameState::Update(float deltaTime)
{
	if (!mPauseAnimation)
	{
		mCharacterAnimatorA.Update(deltaTime);
		mCharacterAnimatorB.Update(deltaTime);
		mCharacterAnimatorC.Update(deltaTime);

		float prevTime = mAnimationTime;
		mAnimationTime += deltaTime;
		mEvents.PlayEvents(prevTime, mAnimationTime);
		while (mAnimationTime >= mAnimationA.GetDuration())
		{
			mAnimationTime -= mAnimationA.GetDuration();
		}
	}

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
	for (auto& ro : mCharacterA)
	{
		ro.transform = mAnimationA.GetTransform(mAnimationTime);
	}

	for (auto& ro : mCharacterB)
	{
		ro.transform = mAnimationB.GetTransform(mAnimationTime);
	}

	for (auto& ro : mCharacterC)
	{
		ro.transform = mAnimationC.GetTransform(mAnimationTime);
	}

	if (!mPauseAnimation)
	{
		mCamera.SetPosition(mCameraAnimation.GetTransform(mAnimationTime).position);
	}

	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);


	mStandardEffect.Begin();
		DrawRenderGroup(mStandardEffect, mCharacterA);
		DrawRenderGroup(mStandardEffect, mCharacterB);
		DrawRenderGroup(mStandardEffect, mCharacterC);
		mStandardEffect.Render(mSky);
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

		if (ImGui::CollapsingHeader("CameraTransform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			Vector3 position = mCamera.GetPosition();
			ImGui::DragFloat3("Position", &position.x, 0.1f);
		}

		ImGui::DragFloat("Time", &mAnimationTime, 0.1f);

		ImGui::Checkbox("PauseAnimation", &mPauseAnimation);
		mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::ChangeAnimation(int animId, Animator& animator)
{
	animator.PlayAnimation(animId, true);
}

void GameState::IdleAnimationA()
{
	 ChangeAnimation(2, mCharacterAnimatorA);
}

void GameState::WavingAnimationA()
{
	ChangeAnimation(3, mCharacterAnimatorA);
}

void GameState::ArgueAnimationA()
{
	ChangeAnimation(4, mCharacterAnimatorA);
}

void GameState::ArgueAnimationB()
{
	ChangeAnimation(2, mCharacterAnimatorB);
}

void GameState::DeathAnimationB()
{
	ChangeAnimation(3, mCharacterAnimatorB);
}

void GameState::DeathLastFrameB()
{
	ChangeAnimation(4, mCharacterAnimatorB);
}

void GameState::AttackAnimationC()
{
	ChangeAnimation(2, mCharacterAnimatorC);
}

void GameState::SetCameraLookAtA()
{
	Vector3 targetPosition = mCharacterA[0].transform.position;
	targetPosition.y += 1.2;
	mCamera.SetLookAt(targetPosition);
}

void GameState::SetCameraLookAtB()
{
	Vector3 targetPosition = mCharacterB[0].transform.position;
	targetPosition.y += 1.2f;
	mCamera.SetLookAt(targetPosition);
}

void GameState::SetCameraLookAtC()
{
	Vector3 targetPosition = mCharacterC[0].transform.position;
	targetPosition.y += 1.2f;
	mCamera.SetLookAt(targetPosition);
}

void GameState::SetCameraLookAtMiddle()
{
	mCamera.SetLookAt({1.5, 1.0f, -12.0f});
}

void GameState::SetCameraLookAtDeath()
{
	mCamera.SetLookAt({ 4, 0.2f, -11.9f });
}

void GameState::FaceLightingLeft()
{
	mDirectionalLight.direction = Math::Normalize({-1.0f, -1.0f, 1.0f });
}

void GameState::FaceLightingRight()
{
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -0.3f, -0.3f });
}

void GameState::FaceLightingMiddle()
{
	mDirectionalLight.direction = { 0.037f, -0.815f, 0.578f };
}

void GameState::FaceLightingBack()
{
	mDirectionalLight.direction = { 0.269f, -0.283f, -0.921f };
}
