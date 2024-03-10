#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetLookAt({ 5.0f, 0.0f, 0.0f });

	//create all planets here
	CreatePlanet(2,   0,    0, 0, L"../../Assets/Images/planets/sun.jpg");
	CreatePlanet(0.15,   3,    2, 176, L"../../Assets/Images/planets/mercury.jpg");
	CreatePlanet( 0.2, 3.8,    5, 720, L"../../Assets/Images/planets/venus.jpg");
	CreatePlanet( 0.2, 4.5,    8,   3, L"../../Assets/Images/planets/earth/earth.jpg");
	CreatePlanet( 0.2, 5.8,   16,   3, L"../../Assets/Images/planets/mars.jpg");
	CreatePlanet(   1,   8,   96,   1, L"../../Assets/Images/planets/jupiter.jpg");
	CreatePlanet(0.75,  12,  240,   1, L"../../Assets/Images/planets/saturn.jpg");
	CreatePlanet( 0.6,  17,  672,   2, L"../../Assets/Images/planets/uranus.jpg");
	CreatePlanet( 0.5,  21, 1320,   2, L"../../Assets/Images/planets/neptune.jpg");
	CreatePlanet( 0.2,  27, 1984,  50, L"../../Assets/Images/planets/pluto.jpg");

	GraphicsSystem::Get()->InitializeBuffer(sizeof(Math::Matrix4));

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

	GraphicsSystem::Get()->CreateVertexShader<VertexPX>(shaderFilePath);
	GraphicsSystem::Get()->CreatePixelShader(shaderFilePath);

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	for (int i = 0; i < planetCount; i++)
	{
		mTextures[i]->Terminate();
		delete mTextures[i];
		mTextures[i] = nullptr;
		delete mInfo[i];
		mInfo[i] = nullptr;
	}
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

	for (int i = 1; i < planetCount; i++)
	{
		mInfo[i]->angle += deltaTime * 2.0 * Math::Constants::Pi / mInfo[i]->orbitTime;
		mInfo[i]->position.x = mInfo[i]->orbitRadius * cos(mInfo[i]->angle);
		mInfo[i]->position.z = mInfo[i]->orbitRadius * sin(mInfo[i]->angle);
	}
}

void GameState::Render()
{
	for (int i = 0; i < planetCount; i++)
	{
		mTextures[i]->BindVS(0);
		mTextures[i]->BindPS(0);

		Math::Matrix4 matTranslate = Math::Matrix4::Translation(mInfo[i]->position);
		Math::Matrix4 matView = mCamera.GetViewMatrix();
		Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
		Math::Matrix4 matFinal = matTranslate * matView * matProj;
		Math::Matrix4 wvp = Math::Transpose(matFinal);

		GraphicsSystem::Get()->UpdateBuffer(&wvp);
		GraphicsSystem::Get()->Render(i);
	}
}

void GameState::CreatePlanet(float planetRadius, float orbitRadius, float orbitTime, float dayTime, std::filesystem::path texturePath)
{
	MeshPX mMesh = MeshBuilder::CreateSpherePX(100, 100, planetRadius);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	Texture* texture = new Texture();
	texture->Initialize(texturePath);	
	mTextures.push_back(texture);

	PlanetInfo* info = new PlanetInfo();
	info->position = { orbitRadius, 0, 0 };
	info->orbitRadius = orbitRadius;
	info->orbitTime = orbitTime;
	info->dayTime = dayTime;
	info->angle = 0;

	mInfo.push_back(info);

	planetCount++;
}
