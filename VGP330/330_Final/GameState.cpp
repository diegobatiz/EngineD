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

	mDepthMapEffect.Initialize();
	mDepthMapEffect.SetCamera(mCamera);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFilePath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mDepthMapEffect.GetCamera());
	mStandardEffect.SetShadowMap(mDepthMapEffect.GetDepthMap());

	shaderFilePath = L"../../Assets/Shaders/PostProcessing.fx";
	mPostProcessEffect.Initialize(shaderFilePath);
	mPostProcessEffect.SetTexture(&mComicBookEffect.GetResultTexture());
	mPostProcessEffect.SetTexture(&mComicBookEffect.GetResultTexture(), 1);
	mPostProcessEffect.SetMode(PostProcessingEffect::Mode::Combine2);

	shaderFilePath = L"../../Assets/Shaders/ComicBook.fx";
	mComicBookEffect.Initialize(shaderFilePath);
	mComicBookEffect.SetSourceTexture(mLineRenderTarget);

	Graphics_D3D11* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mLineRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mComicBookEffect.Terminate();
	mDepthMapEffect.Terminate();
	mStandardEffect.Terminate();
	mPostProcessEffect.Terminate();
	mScreenQuad.Terminate();
	mGround.Terminate();
	mRenderTarget.Terminate();
	mLineRenderTarget.Terminate();
	CleanupRenderGroup(mCharacter);
}

void GameState::Update(float deltaTime)
{
	Camera* depthCamera = &mDepthMapEffect.GetCamera();
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
		depthCamera->Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
		depthCamera->Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
		depthCamera->Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
		depthCamera->Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
		depthCamera->Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
		depthCamera->Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		depthCamera->Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
		depthCamera->Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			DrawRenderGroup(mStandardEffect, mCharacter);
			mStandardEffect.Render(mGround);
		mStandardEffect.End();
	mRenderTarget.EndRender();

	mLineRenderTarget.BeginRender();
		mStandardEffect.Begin();
			DrawRenderGroup(mStandardEffect, mCharacter);
			mStandardEffect.Render(mGround);
		mStandardEffect.End();
	mLineRenderTarget.EndRender();

	mComicBookEffect.Begin();
		mComicBookEffect.Render(mScreenQuad);
	mComicBookEffect.End();

	mPostProcessEffect.Begin();
		mPostProcessEffect.Render(mScreenQuad);
	mPostProcessEffect.End();
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
	ImGui::Text("Render Target:");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0,0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	);

	mStandardEffect.DebugUI();
	mPostProcessEffect.DebugUI();
	mDepthMapEffect.DebugUI();
	mComicBookEffect.DebugUI();
	ImGui::End();
}
