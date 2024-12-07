#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetLookAt({ 5.0f, 0.0f, 0.0f });
	mCamera2.SetAspectRatio(1.0f);
	//create all planets here
	CreatePlanet(   2,   0,    0,   0, L"../../Assets/Images/planets/sun.jpg");
	CreatePlanet(0.15,   3,    2, 176, L"../../Assets/Images/planets/mercury.jpg");
	CreatePlanet( 0.2, 3.8,    5, 720, L"../../Assets/Images/planets/venus.jpg");
	CreatePlanet( 0.2, 4.5,    8,   3, L"../../Assets/Images/planets/earth/earth.jpg");
	CreatePlanet( 0.2, 5.8,   16,   3, L"../../Assets/Images/planets/mars.jpg");
	CreatePlanet(   1,   8,   96,   1, L"../../Assets/Images/planets/jupiter.jpg");
	CreatePlanet(0.75,  12,  240,   1, L"../../Assets/Images/planets/saturn.jpg");
	CreatePlanet( 0.6,  17,  672,   2, L"../../Assets/Images/planets/uranus.jpg");
	CreatePlanet( 0.5,  21, 1320,   2, L"../../Assets/Images/planets/neptune.jpg");
	CreatePlanet( 0.2,  27, 1984,  50, L"../../Assets/Images/planets/pluto.jpg");

	CreateSkySphere(L"../../Assets/Images/skysphere/CloudySky.jpg");

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
		delete mInfo[i];
		mRenderTargets[i]->Terminate();
		delete mRenderTargets[i];
	}
}

void GameState::Update(float deltaTime)
{
#pragma region Camera Stuff
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

	for (int i = 1; i < planetCount; i++)
	{
		mInfo[i]->orbitAngle /= mInfo[i]->orbitMultiplier;
		mInfo[i]->orbitAngle += deltaTime * 2.0 * Math::Constants::Pi / mInfo[i]->orbitTime;
		mInfo[i]->orbitAngle *= mInfo[i]->orbitMultiplier;

		mInfo[i]->position.x = mInfo[i]->orbitRadius * cos(mInfo[i]->orbitAngle);
		mInfo[i]->position.z = mInfo[i]->orbitRadius * sin(mInfo[i]->orbitAngle);

		if (mInfo[i]->orbitAngle >= 360.0f)
		{
			mInfo[i]->orbitAngle = 0.0f;
		}

		mInfo[i]->rotationAngle /= mInfo[i]->dayMultiplier;
		mInfo[i]->rotationAngle += deltaTime * 2.0 * Math::Constants::Pi / mInfo[i]->dayTime;
		mInfo[i]->rotationAngle *= mInfo[i]->dayMultiplier;
	}
}

void GameState::Render()
{
	for (int i = 0; i < planetCount; i++)
	{
		mRenderTargets[i]->BeginRender();
		RenderPlanetImage(i);
		mRenderTargets[i]->EndRender();

		RenderPlanet(i);
	}

	mTextures[planetCount]->BindVS(0);
	mTextures[planetCount]->BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matView * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);

	GraphicsSystem::Get()->UpdateBuffer(&wvp);
	GraphicsSystem::Get()->Render(planetCount);
}

void GameState::CreatePlanet(float planetRadius, float orbitRadius, float orbitTime, float dayTime, std::filesystem::path texturePath)
{
	MeshPX mMesh = MeshBuilder::CreateSpherePX(100, 100, planetRadius);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	Texture* texture = new Texture();
	texture->Initialize(texturePath);
	mTextures.push_back(texture);

	RenderTarget* renderTarget = new RenderTarget();
	constexpr uint32_t size = 512;
	renderTarget->Initialize(size, size, Texture::Format::RGBA_U32);
	mRenderTargets.push_back(renderTarget);

	PlanetInfo* info = new PlanetInfo();
	info->position = { orbitRadius, 0, 0 };
	info->orbitRadius = orbitRadius;
	info->orbitTime = orbitTime;
	info->dayTime = dayTime;
	info->orbitAngle = 0;
	info->rotationAngle = 0;
	info->orbitMultiplier = 1.0f;
	info->dayMultiplier = 1.0f;

	mInfo.push_back(info);

	planetCount++;
}

void GameState::RenderPlanet(int i)
{
	mTextures[i]->BindVS(0);
	mTextures[i]->BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	Math::Matrix4 matTranslate = Math::Matrix4::Translation(mInfo[i]->position);
	Math::Matrix4 matRotation = Math::Matrix4::RotationY(mInfo[i]->rotationAngle);
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matRotation * matTranslate * matView * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);

	GraphicsSystem::Get()->UpdateBuffer(&wvp);
	GraphicsSystem::Get()->Render(i);
}

void GameState::RenderPlanetImage(int i)
{
	mTextures[i]->BindVS(0);
	mTextures[i]->BindPS(0);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mCamera2.SetPosition({0, 0, -2});
	mCamera2.SetLookAt({0, 0, 0});

	Math::Matrix4 matView = mCamera2.GetViewMatrix();
	Math::Matrix4 matProj = mCamera2.GetProjectionMatrix();
	Math::Matrix4 matFinal = matView * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);

	GraphicsSystem::Get()->UpdateBuffer(&wvp);
	GraphicsSystem::Get()->Render(i);
}

void GameState::CreateSkySphere(std::filesystem::path texturePath)
{
	MeshPX mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 1000);

	GraphicsSystem::Get()->CreateMeshBuffer(mMesh);

	Texture* texture = new Texture();
	texture->Initialize(texturePath);
	mTextures.push_back(texture);
}

bool drawRings = false;
void GameState::DebugUI()
{
	DebugUI::SetTheme(DebugUI::Theme::Dark);
	ImGui::Begin("Solar System Model", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::LabelText(" ", "Edit Speed");

	if (ImGui::Button("Toggle Rings"))
	{
		drawRings = !drawRings;
	}

	ImGui::Image(
		mRenderTargets[1]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Mercury", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Mercury", &mInfo[1]->orbitMultiplier, 0.01f, 0.1f, 5.0f);
		ImGui::DragFloat("Day Speed Multiplier##Mercury", &mInfo[1]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[2]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Venus", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Venus", &mInfo[2]->orbitMultiplier, 0.01f, 0.1f, 5.0f);
		ImGui::DragFloat("Day Speed Multiplier##Venus", &mInfo[2]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[3]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Earth", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Earth", &mInfo[3]->orbitMultiplier, 0.01f, 0.1f, 6.0f);
		ImGui::DragFloat("Day Speed Multiplier##Earth", &mInfo[3]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[4]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Mars", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Mars", &mInfo[4]->orbitMultiplier, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat("Day Speed Multiplier##Mars", &mInfo[4]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[5]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Jupiter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Jupiter", &mInfo[5]->orbitMultiplier, 0.1f, 0.1f, 24.0f);
		ImGui::DragFloat("Day Speed Multiplier##Jupiter", &mInfo[5]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[6]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Saturn", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Saturn", &mInfo[6]->orbitMultiplier, 0.1f, 0.1f, 60.0f);
		ImGui::DragFloat("Day Speed Multiplier##Saturn", &mInfo[6]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[7]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Uranus", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Uranus", &mInfo[7]->orbitMultiplier, 1.0f, 0.1f, 160.0f);
		ImGui::DragFloat("Day Speed Multiplier##Uranus", &mInfo[7]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[8]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Neptune", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Neptune", &mInfo[8]->orbitMultiplier, 1.0f, 0.1f, 200.0f);
		ImGui::DragFloat("Day Speed Multiplier##Neptune", &mInfo[8]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::Image(
		mRenderTargets[9]->GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1,1 },
		{ 1, 1, 1, 1 },
		{ 1,1,1,1 }
	);

	if (ImGui::CollapsingHeader("Pluto", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Orbit Speed Multiplier##Pluto", &mInfo[9]->orbitMultiplier, 1.0f, 0.1f, 500.0f);
		ImGui::DragFloat("Day Speed Multiplier##Pluto", &mInfo[9]->dayMultiplier, 0.01f, 0.01f, 2.0f);
	}

	ImGui::End();

	if (drawRings)
	{
		for (int i = 0; i < planetCount; i++)
		{
			SimpleDraw::AddGroundCircle(100, mInfo[i]->orbitRadius, Colors::White);
			SimpleDraw::Render(mCamera);
		}
	}
}
