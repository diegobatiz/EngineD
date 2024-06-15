#include "Precompiled.h"
#include "NormalMapEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void NormalMapEffect::Initialize()
{	 
	std::filesystem::path shaderFile = L"../../Assets/Shaders/NormalMap.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}	 
	 
void NormalMapEffect::Terminate()
{	 
	mDepthMapRenderTarget.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}	 
	 
void NormalMapEffect::Begin()
{	 
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);

	mDepthMapRenderTarget.BeginRender();
}	 
	 
void NormalMapEffect::End()
{	 
	mDepthMapRenderTarget.EndRender();
}	 
	 
void NormalMapEffect::Render(const RenderObject& renderObject)
{	 
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera.GetViewMatrix();
	const Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}	 
	 
void NormalMapEffect::DebugUI()
{	 
	if (ImGui::CollapsingHeader("NormalMapEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("NormalMap");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{ 144,144 },
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 1, 1, 1 },
			{ 1, 1, 1, 1 });
		ImGui::DragFloat("Size##NormalMap", &mSize, 1.0f, 1.0f, 1000.0f);
	}
}	 
	 
void NormalMapEffect::SetSize(float size)
{
	mSize = size;
}

void NormalMapEffect::SetCamera(Camera camera)
{
	mCamera = camera;
	mCamera.SetNearPlane(0.2f);
	mCamera.SetFarPlane(50.0f);
}

Camera& NormalMapEffect::GetCamera()
{
	return mCamera;
}

const Texture& NormalMapEffect::GetNormalMap() const
{
	return mDepthMapRenderTarget;
}
