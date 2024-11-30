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

	constexpr uint32_t resolution = 1024;
	mPlayerPositionRenderTarget.Initialize(resolution, resolution, Texture::Format::RGBA_U32);

	mPositionBuffer.Initialize();
}	 
	 
void PlayerPositionMapEffect::Terminate()
{	 
	mPositionBuffer.Terminate();
	mPlayerPositionRenderTarget.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}	 
	 
void PlayerPositionMapEffect::Begin()
{	 
	mVertexShader.Bind();
	mPixelShader.Bind();

	mPositionBuffer.BindPS(0);

	mPlayerPositionRenderTarget.BeginRenderNoClear();
}	 
	 
void PlayerPositionMapEffect::End()
{
	mPlayerPositionRenderTarget.EndRender();
}	 
	 
void PlayerPositionMapEffect::Render(const RenderObject& renderObject)
{	 
	PlayerPosition posData;
	posData.position = { mPlayerTransform->position.x, mPlayerTransform->position.z };
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
		ImGui::Image(
			mPlayerPositionRenderTarget.GetRawData(),
			{ 144,144 },
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 1, 1, 1 },
			{ 1, 1, 1, 1 });
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
	return mPlayerPositionRenderTarget;
}
