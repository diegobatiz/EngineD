#include "Precompiled.h"
#include "FogEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void FogEffect::Initialize(const std::filesystem::path& filename)
{
	mColorBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<VertexPC>(filename);
	mPixelShader.Initialize(filename);

	mColorData.fogColour = { 0.905882353, 0.905882353, 0.905882353, 1.0f };
	mColorData.fogDensity = 0.016f;
	mColorData.fogOffset = 0.0f;
}


void FogEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mColorBuffer.Terminate();
}

void FogEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mColorBuffer.BindPS(1);

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
	colorData.fogColour = mColorData.fogColour;
	colorData.fogDensity = mColorData.fogDensity;
	colorData.fogOffset = mColorData.fogOffset;

	mColorBuffer.Update(colorData);
}

void FogEffect::End()
{
}

void FogEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void FogEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void FogEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("FogEffect"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4("Fog Color##FogEffect", &mColorData.fogColour.r);
		ImGui::DragFloat("Fog Density##FogEffect", &mColorData.fogDensity, 0.001f, 0.00001f, 1.0f);
		ImGui::DragFloat("Fog Offset##FogEffect", &mColorData.fogOffset, 0.1f, 0.01f, 100.0f);
	}
}