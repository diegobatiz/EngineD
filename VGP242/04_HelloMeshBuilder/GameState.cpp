#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//mMesh = MeshBuilder::CreatePyramidPC(2.0f);
	//mMesh = MeshBuilder::CreateCubePC(2.0f);
	//mMesh = MeshBuilder::CreateRectPC(2.0f, 4.0f, 1.0f);
	//mMesh = MeshBuilder::CreateVerticalPlanePC(10, 10, 1.0f);
	//mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);
	//mMesh = MeshBuilder::CreateCylinderPC(100, 2);
	//mMesh = MeshBuilder::CreateSpherePC(100, 100, 1.0f);

	//mMesh = MeshBuilder::CreateVerticalPlanePX(10, 10, 1.0f);
	//mMesh = MeshBuilder::CreateHorizontalPlanePX(10, 10, 1.0f);

	//mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
	mMesh = MeshBuilder::CreateSkyBoxPX(100.0f);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	GraphicsSystem::Get()->InitializeBuffer(sizeof(Math::Matrix4));

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPX>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);

	mTexture.Initialize(L"../../Assets/Images/skybox/skybox_texture.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	mTexture.Terminate();
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