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
	mTrailBuffer.Initialize();
	mTimeBuffer.Initialize();
}	 
	 
void PlayerPositionMapEffect::Terminate()
{	 
	mTimeBuffer.Terminate();
	mTrailBuffer.Terminate();
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
	mTrailBuffer.BindPS(1);
	mTimeBuffer.BindPS(2);

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
	 
void PlayerPositionMapEffect::Update(float deltaTime)
{
	mTime += deltaTime;
}

void PlayerPositionMapEffect::Render(const RenderObject& renderObject)
{	 
	PlayerPosition posData;
	posData.position = { mPlayerTransform->position.x, -mPlayerTransform->position.z };
	posData.position += { mSnowWidth * 0.5f, mSnowHeight * 0.5f };
	posData.position.x /= mSnowWidth;
	posData.position.y /= mSnowHeight;
	posData.playerRadius = mRadius / mSnowWidth;
	mPositionBuffer.Update(posData);

	TrailSettings trail;
	trail.startGradient = mTrailSettings.startGradient;
	trail.edgeThickness = mTrailSettings.edgeThickness;
	mTrailBuffer.Update(trail);

	Time time;
	time.time = mTime;
	mTimeBuffer.Update(time);

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

		ImGui::DragFloat("Min Start Gradient", &mTrailSettings.minStartGradient, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Max Start Gradient", &mTrailSettings.maxStartGradient, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Start Gradient", &mTrailSettings.startGradient, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Edge Thickness", &mTrailSettings.edgeThickness, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Radius", &mRadius, 0.01f, 0.01f, 10.0f);
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
