#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	CreateBuffer();

	GraphicsSystem::Get()->InitializeBuffer(sizeof(Math::Matrix4));

	CreateShader();
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

void CubeState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("RectState");
	}
}

void CubeState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateCubePC(4.0f);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
}

void CubeState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPC>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void RectState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("PlaneState");
	}
}

void RectState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateRectPC(1.0f, 1.0f, 6.0f);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
}

void RectState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPC>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void PlaneState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("SphereState");
	}
}

void PlaneState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateHorizontalPlanePC(15, 15, 1.0f);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
}

void PlaneState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPC>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void SphereState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("CylinderState");
	}
}

void SphereState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateSpherePC(50, 50, 2.0f);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
}

void SphereState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPC>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void CylinderState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("SkyBoxState");
	}
}

void CylinderState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateCylinderPC(90, 1);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);
}

void CylinderState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransform.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPC>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void SkyBoxState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("SkySphereState");
	}
}

void SkyBoxState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateSkyBoxPX(100);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	mTexture.Initialize(L"../../Assets/Images/skybox/skybox_texture.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SkyBoxState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPX>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}

void SkySphereState::Update(float deltaTime)
{
	GameState::Update(deltaTime);

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		MainApp().ChangeState("CubeState");
	}
}

void SkySphereState::CreateBuffer()
{
	mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	mTexture.Initialize(L"../../Assets/Images/skysphere/space.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SkySphereState::CreateShader()
{
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPX>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);
}
