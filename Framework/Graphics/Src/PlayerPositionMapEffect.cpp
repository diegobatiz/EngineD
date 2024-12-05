#include "Precompiled.h"
#include "PlayerPositionMapEffect.h"

#include "RenderObject.h"
#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void PlayerPositionMapEffect::Initialize()
{	 
	std::filesystem::path shaderFile = L"../../Assets/Shaders/PositionMap.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);

	constexpr uint32_t resolution = 4096;
	mPlayerPositionRenderTargetA.Initialize(resolution, resolution, Texture::Format::RGBA_U32);
	mPlayerPositionRenderTargetB.Initialize(resolution, resolution, Texture::Format::RGBA_U32);

	mPositionBuffer.Initialize();
}	 
	 
void PlayerPositionMapEffect::Terminate()
{	 
	mPositionBuffer.Terminate();
	mPlayerPositionRenderTargetB.Terminate();
	mPlayerPositionRenderTargetA.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}	 
	 
void PlayerPositionMapEffect::Begin()
{	 
	mVertexShader.Bind();
	mPixelShader.Bind();

	mPositionBuffer.BindPS(0);

	if (mUseA)
	{
		mPlayerPositionRenderTargetB.BindPS(0);
		mPlayerPositionRenderTargetA.BeginRenderNoClear();
	}
	else
	{
		mPlayerPositionRenderTargetA.BindPS(0);
		mPlayerPositionRenderTargetB.BeginRenderNoClear();
	}

	mSampler.BindPS(0);
}	 
	 
void PlayerPositionMapEffect::End()
{
	if (mUseA)
	{
		mPlayerPositionRenderTargetA.EndRender();
		mUseA = false;
	}
	else
	{
		mPlayerPositionRenderTargetB.EndRender();
		mUseA = true;
	}

	Texture::UnbindPS(0);
}	 
	 
void PlayerPositionMapEffect::Render(const RenderObject& renderObject)
{	 
	PlayerPosition posData;
	posData.position = { mPlayerTransform->position.x, -mPlayerTransform->position.z };
	posData.position += { mSnowWidth * 0.5f, mSnowHeight * 0.5f };
	posData.playerRadius = 0.5f;
	posData.position.x /= mSnowWidth;
	posData.position.y /= mSnowHeight;
	posData.playerRadius = mRadius / mSnowWidth;
	mPositionBuffer.Update(posData);

	renderObject.meshBuffer.Render();
}	 
	 
void PlayerPositionMapEffect::DebugUI()
{	 
	if (ImGui::CollapsingHeader("Position Map", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (mUseA)
		{
			ImGui::Image(
				mPlayerPositionRenderTargetA.GetRawData(),
				{ 144,144 },
				{ 0, 0 },
				{ 1, 1 },
				{ 1, 1, 1, 1 },
				{ 1, 1, 1, 1 });
		}
		else
		{
			ImGui::Image(
				mPlayerPositionRenderTargetB.GetRawData(),
				{ 144,144 },
				{ 0, 0 },
				{ 1, 1 },
				{ 1, 1, 1, 1 },
				{ 1, 1, 1, 1 });
		}
	}
}

void PlayerPositionMapEffect::SetRadius(float radius)
{
	mRadius = radius;
}

void PlayerPositionMapEffect::SetSnowDimensions(float width, float height)
{
	mSnowWidth = width;
	mSnowHeight = height;
}

const Texture& PlayerPositionMapEffect::GetPositionMap() const
{
	if (mUseA)
	{
		return mPlayerPositionRenderTargetA;
	}
	else
	{
		return mPlayerPositionRenderTargetB;
	}
}
