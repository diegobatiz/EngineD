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
	trail.snowPower = mTrailSettings.snowPower;
	trail.growSnow = mTrailSettings.growSnow;
	trail.resetSnow = mTrailSettings.resetSnow;
	trail.maxStartGradient = mTrailSettings.maxStartGradient;
	trail.minStartGradient = mTrailSettings.minStartGradient;
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
				{ 256,256 },
				{ 0, 0 },
				{ 1, 1 },
				{ 1, 1, 1, 1 },
				{ 1, 1, 1, 1 });
		}
		else
		{
			ImGui::Image(
				mPlayerPositionRenderTargetB.GetRawData(),
				{ 256,256 },
				{ 0, 0 },
				{ 1, 1 },
				{ 1, 1, 1, 1 },
				{ 1, 1, 1, 1 });
		}

		ImGui::DragFloat("Min Start Gradient", &mTrailSettings.minStartGradient, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Max Start Gradient", &mTrailSettings.maxStartGradient, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Edge Power", &mTrailSettings.snowPower, 0.01f, 0.01f, 0.99f);
		ImGui::DragFloat("Radius", &mRadius, 0.01f, 0.01f, 10.0f);
		if (ImGui::Button("Make Snow Fill"))
		{
			if (mTrailSettings.growSnow == 1.0f)
			{
				mTrailSettings.growSnow = 0.0f;
			}
			else
			{
				mTrailSettings.growSnow = 1.0f;
			}
		}
		if (ImGui::Button("Make Trails"))
		{
			if (mTrailSettings.resetSnow == 1.0f)
			{
				mTrailSettings.resetSnow = 0.0f;
			}
			else
			{
				mTrailSettings.resetSnow = 1.0f;
			}
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
