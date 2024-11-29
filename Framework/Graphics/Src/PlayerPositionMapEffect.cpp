#include "Precompiled.h"
#include "PlayerPositionMapEffect.h"

#include "RenderObject.h"
#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void PlayerPositionMapEffect::Initialize()
{	 
	std::filesystem::path shaderFile = L"../../Assets/Shaders/PositionMap.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	constexpr uint32_t depthMapResolution = 4096;
	mPlayerPositionRenderTarget.SimpleInitialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);

	mPositionBuffer.Initialize();
}	 
	 
void PlayerPositionMapEffect::Terminate()
{	 
	mPositionBuffer.Terminate();
	mPlayerPositionRenderTarget.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}	 
	 
void PlayerPositionMapEffect::Begin()
{	 
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mPositionBuffer.BindPS(1);

	mPlayerPositionRenderTarget.SetRenderTarget();
}	 
	 
void PlayerPositionMapEffect::End()
{
}	 
	 
void PlayerPositionMapEffect::Render(const RenderObject& renderObject)
{	 
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	PlayerPosition posData;
	posData.position = mPlayerTransform->position;
	posData.position.x /= mSnowWidth;
	posData.position.y /= mSnowHeight;
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

void PlayerPositionMapEffect::SetSnowDimensions(float width, float height)
{
	mSnowWidth = width;
	mSnowHeight = height;
}

const Texture& PlayerPositionMapEffect::GetPositionMap() const
{
	return mPlayerPositionRenderTarget;
}
	 
void PlayerPositionMapEffect::SetPlayerTransform(const Transform& player)
{
	mPlayerTransform = &player;
}
