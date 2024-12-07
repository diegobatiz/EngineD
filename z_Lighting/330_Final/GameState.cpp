#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 2.0f, -4.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	Model model;
	ModelIO::LoadModel("../../Assets/Models/Character_01/Ch44_nonPBR.model", model);
	ModelIO::LoadMaterial("../../Assets/Models/Character_01/Ch44_nonPBR.model", model);
	mCharacter = CreateRenderGroup(model);

	Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("planets/earth/rock.jpg");

	MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenQuad);

	mNormalMapEffect.Initialize();
	mNormalMapEffect.SetCamera(mCamera);

	mDepthMapEffect.Initialize();
	mDepthMapEffect.SetCamera(mNormalMapEffect.GetCamera());

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/CrosshatchEffect.fx";
	mCrosshatchEffect.Initialize(shaderFilePath);
	mCrosshatchEffect.SetCamera(mCamera);
	mCrosshatchEffect.SetDirectionalLight(mDirectionalLight);
	mCrosshatchEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mCrosshatchEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mTexture.Initialize(L"../../Assets/Images/misc/crosshatchPattern.jpg");
	shaderFilePath = L"../../Assets/Shaders/MoebiusEffect.fx";
	mMoebiusEffect.Initialize(shaderFilePath);
	mMoebiusEffect.SetTexture(&mTexture, 0);
	mMoebiusEffect.SetTexture(&mComicRenderTarget, 1);
	mMoebiusEffect.SetTexture(&mShadowRenderTarget, 2);

	shaderFilePath = L"../../Assets/Shaders/ComicBook.fx";
	mComicBookEffect.Initialize(shaderFilePath);
	mComicBookEffect.SetNormalTexture(mNormalMapEffect.GetNormalMap());
	mComicBookEffect.SetDepthTexture(mDepthMapEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	Graphics_D3D11* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mComicRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mShadowRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mMoebiusEffect.Terminate();
	mComicRenderTarget.EndRender();
	mShadowEffect.Terminate();
	mDepthMapEffect.Terminate();
	mComicBookEffect.Terminate();
	mNormalMapEffect.Terminate();
	mCrosshatchEffect.Terminate();
	mScreenQuad.Terminate();
	mGround.Terminate();
	CleanupRenderGroup(mCharacter);
	mTexture.Terminate();
}

void GameState::Update(float deltaTime)
{
	Camera* normalCamera = &mNormalMapEffect.GetCamera();
	Camera* depthCamera = &mDepthMapEffect.GetCamera();
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
		depthCamera->Walk(moveSpeed * deltaTime);
		normalCamera->Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
		depthCamera->Walk(-moveSpeed * deltaTime);
		normalCamera->Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
		depthCamera->Strafe(moveSpeed * deltaTime);
		normalCamera->Strafe(moveSpeed * deltaTime);
		offsetX = deltaTime;
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
		depthCamera->Strafe(-moveSpeed * deltaTime);
		normalCamera->Strafe(-moveSpeed * deltaTime);
		offsetX -= deltaTime;
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
		depthCamera->Rise(moveSpeed * deltaTime);
		normalCamera->Rise(moveSpeed * deltaTime);
		offsetY -= deltaTime;
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
		depthCamera->Rise(-moveSpeed * deltaTime);
		normalCamera->Rise(-moveSpeed * deltaTime);
		offsetY += deltaTime;
	}

	mMoebiusEffect.Update(offsetX, offsetY);

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		depthCamera->Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		normalCamera->Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
		depthCamera->Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
		normalCamera->Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	mShadowEffect.Begin();
		DrawRenderGroup(mShadowEffect, mCharacter);
	mShadowEffect.End();

	mNormalMapEffect.Begin();
		DrawRenderGroup(mNormalMapEffect, mCharacter);
		mNormalMapEffect.Render(mGround);
	mNormalMapEffect.End();

	mDepthMapEffect.Begin();
		DrawRenderGroup(mDepthMapEffect, mCharacter);
		mDepthMapEffect.Render(mGround);
	mDepthMapEffect.End();

	mComicRenderTarget.BeginRender();
		mComicBookEffect.Begin();
			mComicBookEffect.Render(mScreenQuad);
		mComicBookEffect.End();
	mComicRenderTarget.EndRender();

	mShadowRenderTarget.BeginRender();
		mCrosshatchEffect.Begin();
			DrawRenderGroup(mCrosshatchEffect, mCharacter);
			mCrosshatchEffect.Render(mGround);
		mCrosshatchEffect.End();
	mShadowRenderTarget.EndRender();

	mMoebiusEffect.Begin();
		mMoebiusEffect.Render(mScreenQuad);
	mMoebiusEffect.End();
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
	ImGui::Text("Comic Render Target:");
	ImGui::Image(
		mComicRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0,0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	);

	ImGui::Text("Shadow Map:");
	ImGui::Image(
		mShadowEffect.GetDepthMap().GetRawData(),
		{ 128, 128 },
		{ 0,0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	);

	mCrosshatchEffect.DebugUI();
	mNormalMapEffect.DebugUI();
	mDepthMapEffect.DebugUI();
	mComicBookEffect.DebugUI();
	ImGui::End();
}
