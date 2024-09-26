#include "Precompiled.h"
#include "GrassEffect.h"

#include "Camera.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void GrassEffect::Initialize(const std::filesystem::path& filename)
{
	mColorBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize(filename);
	mPixelShader.Initialize(filename);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);

	mColorData.albedo1Colour = { 0.43137255f, 0.80392157f, 0.36862745f, 1.0f };
	mColorData.albedo2Colour = { 0.08235294f, 0.49019608f, 0.16470588f, 1.0f };
	mColorData.AOColour = { 0.31372549f, 0.31372549f, 0.31372549f, 1.0f };
	mColorData.tipColour = { 0.0f, 0.15686275f, 0.0f, 1.0f };
}

void GrassEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mColorBuffer.Terminate();
}

void GrassEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);

	mColorBuffer.BindPS(1);

	mBlendState.Set();

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);

	mTransformBuffer.Update(data);

	ColorData colorData;
	colorData.albedo1Colour = mColorData.albedo1Colour;
	colorData.albedo2Colour = mColorData.albedo2Colour;
	colorData.AOColour = mColorData.AOColour;
	colorData.tipColour = mColorData.tipColour;

	mColorBuffer.Update(colorData);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(mGrassTextureId, 0);


}

void GrassEffect::End()
{
	mBlendState.ClearState();
}

void GrassEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void GrassEffect::SetGrassTextureID(TextureID id)
{
	mGrassTextureId = id;
}

void GrassEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("GrassEffect"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4("Albedo Top", &mColorData.albedo1Colour.r);
		ImGui::ColorEdit4("Albedo Bottom", &mColorData.albedo2Colour.r);
		ImGui::ColorEdit4("Ambient Occlusion", &mColorData.AOColour.r);
		ImGui::ColorEdit4("Tip Color", &mColorData.tipColour.r);
	}
}
