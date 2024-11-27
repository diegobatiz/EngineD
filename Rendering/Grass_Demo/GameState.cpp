#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 15.0f, -50.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	GraphicsSystem::Get()->SetClearColor(Colors::SkyBlue);

	////////////////////////////////////////
	//        Billboard Grass Mesh       //
	///////////////////////////////////////

	/*
	MeshPX mesh;
	const float hw = 1.0f * 0.5f;
	const float h = 1.0f;
	const float qSqr2 = sqrt(2.0f) * 0.25f;

	mesh.vertices.push_back({ {-hw, 0.0f, 0.0f}, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-hw,  h, 0.0f}, {0.0f, 0.0f} });
	mesh.vertices.push_back({ { hw,  h, 0.0f}, {1.0f, 0.0f} });
	mesh.vertices.push_back({ { hw, 0.0f, 0.0f}, {1.0f, 1.0f} });

	mesh.vertices.push_back({ {-qSqr2, 0.0f, qSqr2}, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-qSqr2,  h, qSqr2}, {0.0f, 0.0f} });
	mesh.vertices.push_back({ { qSqr2,  h, -qSqr2}, {1.0f, 0.0f} });
	mesh.vertices.push_back({ { qSqr2, 0.0f, -qSqr2}, {1.0f, 1.0f} });

	mesh.vertices.push_back({ {-qSqr2, 0.0f, -qSqr2}, {0.0f, 1.0f} });
	mesh.vertices.push_back({ {-qSqr2,  h, -qSqr2}, {0.0f, 0.0f} });
	mesh.vertices.push_back({ {qSqr2,  h, qSqr2}, {1.0f, 0.0f} });
	mesh.vertices.push_back({ {qSqr2, 0.0f, qSqr2}, {1.0f, 1.0f} });
	

	mesh.indices = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11
	};
	*/

	//SETS TEXTURE FOR BILLBOARD GRASS//
	
	//TextureID id = TextureManager::Get()->LoadTexture("terrain/Grass.png");
	//mGrassEffect.SetGrassTextureID(id);

	int density = 3;
	int sideSize = 250;

	mTerrain.SetOffset(sideSize);
	mTerrain.SetDensity(density);
	mTerrain.Initialize("../../Assets/Images/terrain/heightmap_1024x1024.raw", 5.0f, Colors::DarkGreen);

	const MeshPC& m = mTerrain.GetMesh();
	mGround.meshBuffer.Initialize(
		nullptr,
		static_cast<uint32_t>(sizeof(VertexPC)),
		static_cast<uint32_t>(m.vertices.size()),
		m.indices.data(),
		static_cast<uint32_t>(m.indices.size())
	);
	mGround.meshBuffer.Update(m.vertices.data(), m.vertices.size());

	Model model;
	ModelIO::LoadModel("../../Assets/Models/Grass/Grass.model", model);
	Mesh mMesh = model.meshData[0].mesh;
	mGrassBuffer.SetDensity(density);
	mGrassBuffer.SetSideSize(sideSize);
	mGrassBuffer.SetTerrain(mTerrain);
	mGrassBuffer.Initialize(mMesh);

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/GrassShader.fx";
	mGrassEffect.Initialize(shaderFilePath);
	mGrassEffect.SetCamera(mCamera);
	mGrassEffect.SetFogEffect(mTerrainEffect);

	mTerrainEffect.Initialize(L"../../Assets/Shaders/Fog.fx");
	mTerrainEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mTerrainEffect.Terminate();
	mGrassEffect.Terminate();
	mGrassBuffer.Terminate();
	mGround.Terminate();
}

void GameState::Update(float deltaTime)
{
#pragma region Camera Movement
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

	mGrassEffect.Update(deltaTime);
}

void GameState::Render()
{
	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();

	mGrassEffect.Begin();
		mGrassBuffer.Render();
	mGrassEffect.End();

	int height = GraphicsSystem::Get()->GetBackBufferHeight();
	int width = GraphicsSystem::Get()->GetBackBufferWidth();
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

		mTerrainEffect.DebugUI();
	
		mGrassEffect.DebugUI();

	ImGui::End();
}
