#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mMesh = MeshBuilder::CreateSpherePX(60, 60, 2.0f);
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

	GraphicsSystem::Get()->InitializeBuffer(sizeof(Math::Matrix4));
	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
	GraphicsSystem::Get()->CreateVertexShader<VertexPX>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);

	mTexture.Initialize(L"../../Assets/Images/planets/earth/earth.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
	GraphicsSystem::Get()->ClearBuffer();
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
	mCamera.SetAspectRatio(1.0f);
	mRenderTarget.BeginRender(); 
	RenderObject();
	mRenderTarget.EndRender();

	mCamera.SetAspectRatio(0.0f);
	RenderObject();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Render Target");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);
	ImGui::End();

	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
}

void GameState::RenderObject()
{
	mTexture.BindVS(0);
	mTexture.BindPS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);

	GraphicsSystem::Get()->UpdateBuffer(&wvp);
	GraphicsSystem::Get()->Render();
}
