#include "Precompiled.h"
#include "GrassEffect.h"
#include "GraphicsSystem.h"

#include "Camera.h"
#include "FogEffect.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void GrassEffect::Initialize(const std::filesystem::path& filename)
{
	mGrassBuffer.Initialize();
	mTimeBuffer.Initialize();
	mColorBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<Vertex>(filename);
	mPixelShader.Initialize(filename);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);

	mColorData.albedo1Colour = { 0.43137255f, 0.80392157f, 0.36862745f, 1.0f };
	mColorData.albedo2Colour = { 0.08235294f, 0.49019608f, 0.16470588f, 1.0f };
	mColorData.AOColour = { 0.31372549f, 0.31372549f, 0.31372549f, 1.0f };
	mColorData.tipColour = { 0.0f, 0.15686275f, 0.0f, 1.0f };
	mColorData.fogColour = { 0.905882353, 0.905882353, 0.905882353, 1.0f };
	mColorData.fogDensity = 0.016f;
	mColorData.fogOffset = 0.0f;

	GraphicsSystem::Get()->SetClearColor(mColorData.fogColour);
}

void GrassEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mColorBuffer.Terminate();
	mTimeBuffer.Terminate();
	mGrassBuffer.Terminate();
}

void GrassEffect::Update(float deltaTime)
{
	mCurrentTime += deltaTime;

	if (mCurrentTime >= 60.0f)
	{
		mCurrentTime = 0.0f;
	}
}

void GrassEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mColorBuffer.BindPS(1);

	mTimeBuffer.BindVS(2);

	mGrassBuffer.BindVS(3);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	Math::Vector3 camPos = mCamera->GetPosition();

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.cameraPos = camPos;

	mTransformBuffer.Update(data);

	ColorData colorData;
	colorData.albedo1Colour = mColorData.albedo1Colour;
	colorData.albedo2Colour = mColorData.albedo2Colour;
	colorData.AOColour = mColorData.AOColour;
	colorData.tipColour = mColorData.tipColour;
	colorData.fogColour = mColorData.fogColour;
	colorData.fogDensity = mColorData.fogDensity;
	colorData.fogOffset = mColorData.fogOffset;

	mColorBuffer.Update(colorData);

	TimeData timeData;
	timeData.time = mCurrentTime;
	
	mTimeBuffer.Update(timeData);

	GrassData grassData;
	grassData.extraHeightAdd = mGrassData.extraHeightAdd;
	grassData.extraHeightMult = mGrassData.extraHeightMult;
	grassData.sway1 = mGrassData.sway1;
	grassData.sway2 = mGrassData.sway2;
	grassData.sway3 = mGrassData.sway3;
	mGrassBuffer.Update(grassData);


	//FOR BILLBOARD GRASS//

	//TextureManager* tm = TextureManager::Get();
	//tm->BindPS(mGrassTextureId, 0);
}

void GrassEffect::End()
{
}

void GrassEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

//FOR BILLBOARD GRASS//
void GrassEffect::SetGrassTextureID(TextureID id)
{
	mGrassTextureId = id;
}

void GrassEffect::SetFogEffect(FogEffect& effect)
{
	mFogEffect = &effect;
}

void GrassEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("GrassEffect"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4("Albedo Top", &mColorData.albedo1Colour.r);
		ImGui::ColorEdit4("Albedo Bottom", &mColorData.albedo2Colour.r);
		ImGui::ColorEdit4("Ambient Occlusion", &mColorData.AOColour.r);
		ImGui::ColorEdit4("Tip Color", &mColorData.tipColour.r);
		if (ImGui::ColorEdit4("Fog Color", &mColorData.fogColour.r))
		{
			GraphicsSystem::Get()->SetClearColor(mColorData.fogColour);
			if (mFogEffect != nullptr)
			{
				mFogEffect->SetFogColor(mColorData.fogColour);
			}
		}
		ImGui::DragFloat("Extra Grass Height Add", &mGrassData.extraHeightAdd, 0.001f);
		ImGui::DragFloat("Extra Grass Height Mult", &mGrassData.extraHeightMult, 0.001f);
		ImGui::DragFloat("Sway Variance", &mGrassData.sway1, 0.001f);
		ImGui::DragFloat("Sway Speed", &mGrassData.sway2, 0.001f);
		ImGui::DragFloat("Sway Factor", &mGrassData.sway3, 0.001f);
		ImGui::DragFloat("Fog Density", &mColorData.fogDensity, 0.001f, 0.00001f, 1.0f);
		ImGui::DragFloat("Fog Offset", &mColorData.fogOffset, 0.1f, 0.01f, 100.0f);
	}
	if (ImGui::CollapsingHeader("CameraTransform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Math::Vector3 position = mCamera->GetPosition();
		ImGui::DragFloat3("Position", &position.x, 0.1f);
	}
}
