#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;
using namespace EngineD::Audio;
using namespace EngineD::Physics;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0, 2.5f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	Mesh sky = MeshBuilder::CreateSkySphere(60, 60, 175.0f);
	mSky.meshBuffer.Initialize(sky);
	mSky.diffuseMapId = TextureManager::Get()->LoadTexture("skysphere/CloudySky.jpg");
	mSky.UseLighting(false);

	Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(100, 100, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh); 
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("terrain/ground.jpg");

	Mesh earthMesh = MeshBuilder::CreateSphere(60, 60, 180.0f);
	mEarth.meshBuffer.Initialize(earthMesh);
	mEarth.diffuseMapId = TextureManager::Get()->LoadTexture("planets/earth/earth.jpg");

	mModelIdA = ModelManager::Get()->LoadModelId("../../Assets/Models/Racer/Racer.model");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Walking.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Idle.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Waving.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Arguing.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Running.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Crying.animset");
	ModelManager::Get()->AddAnimation(mModelIdA, "../../Assets/Models/Racer/RacerAnimations/Pointing.animset");
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
	ModelManager::Get()->AddAnimation(mModelIdC, "../../Assets/Models/Guy/GuyAnimations/Dancing.animset");
	ModelManager::Get()->AddAnimation(mModelIdC, "../../Assets/Models/Guy/GuyAnimations/Idle.animset");
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
		.AddRotationKey({ 0, -0.7071, 0, 0.7071 }, 40.0f)
		.AddRotationKey({ 0,  0.7071, 0, 0.7071 }, 41.0f)
		.AddPositionKey({ 0.0f, 0.0f, -12.0f }, 41.0f)
		.AddPositionKey({ -28.0f, 0.0f, -12.0f }, 46.0f)
		.AddRotationKey({ 0,  0.7071, 0, 0.7071 }, 52.0f)
		.AddRotationKey({ 0, -0.7071, 0, 0.7071 }, 53.0f)
		.Build();

	mAnimationB = AnimationBuilder()
		.AddPositionKey({ 4.0f, 0.0f, -12.0f }, 0.0f)
		.AddRotationKey({ 0, 0.7071f, 0, 0.7071f }, 0.0f)
		.AddPositionKey({ 4.0f, 0.0f, -12.0f }, 16.48f)
		.AddPositionKey({ 2.5f, 0.0f, -12.0f }, 16.5f)
		.Build();

	mAnimationC = AnimationBuilder()
		.AddPositionKey({ 2.5f, 0.0f, -30.0f }, 0.0f)
		.AddRotationKey({ 0, 1, 0, 0 }, 0.0f)
		.AddPositionKey({ 2.5f, 0.0f, -40.0f }, 26.0f)
		.AddPositionKey({ 2.5f, 0.0f, -13.3f }, 36.0f)
		.AddPositionKey({ 2.5f, 0.0f, -13.3f }, 38.06)
		.AddPositionKey({ 0.0f, 0.0f, -13.3f }, 38.5f)
		.AddPositionKey({ 0.0f, 0.0f, -13.3f }, 59.0f)
		.AddPositionKey({ 250.0f, 100.0f, -13.3f }, 70.0f, EaseType::EaseOutQuad)
		.AddPositionKey({ 250.0f, 100.0f, -13.3f }, 100.5f)
		.Build();

	mEarthAnimation = AnimationBuilder()
		.AddRotationKey({ 0, -0.7071, 0, 0.7071 }, 0.0f)
		.AddRotationKey({ 0,  0.7071, 0, 0.7071 }, 20.0f)
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
		.AddPositionKey({ 4.1f, 1.13f, -12.92f }, 40.99f)
		.AddPositionKey({ -4.1f, 1.5f, -16.0f }, 41.0f)
		.AddPositionKey({ -4.1f, 1.5f, -16.0f }, 45.95f)
		.AddPositionKey({ -28.418f, 1.679f, -12.53f }, 46.0f)
		.AddPositionKey({ -28.418f, 1.679f, -12.53f }, 51.99f)
		.AddPositionKey({ -1.95f, 1.91f, -12.41f }, 52.0f)
		.AddPositionKey({ -1.95f, 1.91f, -12.41f }, 57.99f)
		.AddPositionKey({ -28.495f, 1.646f, -12.287f }, 58.0f)
		.AddPositionKey({ -28.495f, 1.646f, -12.287f }, 60.99f)
		.AddPositionKey({ -1.95f, 1.91f, -12.41f }, 61.0f)
		.AddPositionKey({ -1.95f, 1.91f, -12.41f }, 62.49f)
		.AddPositionKey({ -0, 400.0f, -500.0f }, 62.5f)
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
		.AddEventKey(std::bind(&GameState::RunAnimationA, this), 41.0f)
		.AddEventKey(std::bind(&GameState::DanceAnimationC, this), 41.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtDistance, this), 41.01f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtA, this), 46.05f)
		.AddEventKey(std::bind(&GameState::FaceLightingRight, this), 46.0f)
		.AddEventKey(std::bind(&GameState::CryAnimationA, this), 46.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtC, this), 52.05f)
		.AddEventKey(std::bind(&GameState::PointAnimationA, this), 58.0f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtC, this), 58.05f)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtC, this), 61.05)
		.AddEventKey(std::bind(&GameState::SetCameraLookAtEarth, this), 62.55f)
		.AddEventKey(std::bind(&GameState::ActivateParticleSystem, this), 66.0f)
		.Build();

	ChangeAnimation(1, mCharacterAnimatorA);
	ChangeAnimation(1, mCharacterAnimatorB);
	ChangeAnimation(1, mCharacterAnimatorC);

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	ParticleSystemInfo info;
	info.maxParticles = 200;
	info.particleTextureId = TextureManager::Get()->LoadTexture("Explosion.png");
	info.spawnPosition = Math::Vector3::Zero;
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.spawnLifeTime = 9999999999999.0f;
	info.minParticlePerEmit = 4;
	info.maxParticlePerEmit = 7;
	info.minTimeBetweenEmit = 0.5f;
	info.maxTimeBetweenEmit = 1.0f;
	info.minSpawnAngle = -130.0f * Math::Constants::Pi / 180.0f;
	info.maxSpawnAngle = 130.0f * Math::Constants::Pi / 180.0f;
	info.minSpeed = 50.0f;
	info.maxSpeed = 100.0f;
	info.minParticleLifetime = 3.0f;
	info.maxParticleLifetime = 6.0f;
	info.minStartColor = Colors::Yellow;
	info.maxStartColor = Colors::Orange;
	info.minEndColor = Colors::DarkRed;
	info.maxEndColor = Colors::Red;
	info.minStartScale = { 1000.0f, 1000.0f, 1000.0f };
	info.maxStartScale = { 1000.0f, 1000.0f, 1000.0f };
	info.minEndScale = { 1500.0f, 1500.0f, 1500.0f };
	info.maxEndScale = { 1500.0f, 1500.0f, 1500.0f };
	mParticleSystem.Initialize(info);
	mParticleSystem.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mParticleSystem.Terminate();
	mParticleEffect.Terminate();
	mStandardEffect.Terminate();
	CleanupRenderGroup(mCharacterC);
	CleanupRenderGroup(mCharacterB);
	CleanupRenderGroup(mCharacterA);
	mEarth.Terminate();
	mGround.Terminate();
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
		while (mAnimationTime >= mAnimationC.GetDuration())
		{
			mAnimationTime -= mAnimationA.GetDuration();
		}

		if (mSpaceView)
		{
			float prevTime = mRotationTime;
			mRotationTime += deltaTime;
			while (mRotationTime >= mEarthAnimation.GetDuration())
			{
				mRotationTime -= mEarthAnimation.GetDuration();
			}
		}
	}

	if (mRenderParticles)
	{
		mParticleSystem.Update(deltaTime);
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

	if (mSpaceView)
	{
		mEarth.transform = mEarthAnimation.GetTransform(mRotationTime);
	}


	mStandardEffect.Begin();
		DrawRenderGroup(mStandardEffect, mCharacterA);
		DrawRenderGroup(mStandardEffect, mCharacterB);
		DrawRenderGroup(mStandardEffect, mCharacterC);
		mStandardEffect.Render(mGround);
		mStandardEffect.Render(mSky);
		if (mSpaceView)
		{
			mStandardEffect.Render(mEarth);
		}
	mStandardEffect.End();

	if (mRenderParticles)
	{
		mParticleEffect.Begin();
			mParticleSystem.Render(mParticleEffect);
		mParticleEffect.End();
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

void GameState::RunAnimationA()
{
	ChangeAnimation(5, mCharacterAnimatorA);
}

void GameState::CryAnimationA()
{
	ChangeAnimation(6, mCharacterAnimatorA);
}

void GameState::PointAnimationA()
{
	ChangeAnimation(7, mCharacterAnimatorA);
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

void GameState::DanceAnimationC()
{
	ChangeAnimation(3, mCharacterAnimatorC);
}

void GameState::IdleAnimationC()
{
	ChangeAnimation(4, mCharacterAnimatorC);
}

void GameState::SetCameraLookAtA()
{
	Vector3 targetPosition = mCharacterA[0].transform.position;
	targetPosition.y += 1.3;
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

void GameState::SetCameraLookAtDistance()
{
	mCamera.SetLookAt({ -80, 1.5f, 100.0f });
}

void GameState::SetCameraLookAtEarth()
{
	mCamera.SetLookAt({ 0, 0, 0 });
	mSpaceView = true;
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
