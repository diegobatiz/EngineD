#include "Precompiled.h"
#include "SnowEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void SnowEffect::Initialize()
{
	mTransformBuffer.Initialize();
	mTessBuffer.Initialize();
	mLightingBuffer.Initialize();

	std::filesystem::path shaderFile = "../../Assets/Shaders/Snow.fx";
	mVertexShader.Initialize<VertexD>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mHullShader.Initialize(shaderFile);
	mDomainShader.Initialize(shaderFile);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SnowEffect::Terminate()
{
	mSampler.Terminate();
	mDomainShader.Terminate();
	mHullShader.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mLightingBuffer.Terminate();
	mTessBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void SnowEffect::Begin()
{
	mVertexShader.Bind();
	mHullShader.Bind();
	mDomainShader.Bind();
	mPixelShader.Bind();

	mSampler.BindDS(0);
	mSampler.BindPS(0);

	mPositionMap->BindDS(0);
	mPositionMap->BindPS(0);

	mTransformBuffer.BindDS(0);

	mTessBuffer.BindHS(1);

	mLightingBuffer.BindPS(2);
}

void SnowEffect::End()
{
	if (mPositionMap != nullptr)
	{
		Texture::UnbindDS(0);
		Texture::UnbindPS(0);
	}
}

void SnowEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Math::Transpose(matFinal);
	transformData.cameraPos = mCamera->GetPosition();

	mTransformBuffer.Update(transformData);

	TessellationData tessData;

	mTessBuffer.Update(tessData);

	LightingSettings lightSettings;
	lightSettings.normalStrength = mLightingSettings.normalStrength;
	mLightingSettings.textureSize = mPositionMap->GetWidth();
	mLightingBuffer.Update(lightSettings);

	renderObject.meshBuffer.Render();
}

void SnowEffect::SetPositionMap(const Texture& posMap)
{
	mPositionMap = &posMap;
}

void SnowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Snow Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Tessellation Data");
		ImGui::DragFloat("Min Tessellation Distance", &mTessData.minTessDistance, 0.1f, 1.0f, 50.0f);
		ImGui::DragFloat("Max Tessellation Distance", &mTessData.maxTessDistance, 0.1f, mTessData.minTessDistance + 1.0f, 100.0f);
		ImGui::DragFloat("Tessellation Level", &mTessData.tessLevel, 1.0f, 1.0f, 8.0f);
	}
}
