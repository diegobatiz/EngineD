#include "Precompiled.h"
#include "PlayerPositionMapEffect.h"

#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void PlayerPositionMapEffect::Initialize()
{	 
	std::filesystem::path shaderFile = L"../../Assets/Shaders/PositionMap.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	constexpr uint32_t depthMapResolution = 4096;
	mPlayerPositionRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}	 
	 
void PlayerPositionMapEffect::Terminate()
{	 
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

	mPlayerPositionRenderTarget.BeginRender();
}	 
	 
void PlayerPositionMapEffect::End()
{
	mPlayerPositionRenderTarget.EndRender();
}	 
	 
void PlayerPositionMapEffect::Render(const RenderObject& renderObject)
{	 
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

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
	 
void PlayerPositionMapEffect::SetPlayerTransform(const Transform& player)
{
	mPlayerTransform = &player;
}
