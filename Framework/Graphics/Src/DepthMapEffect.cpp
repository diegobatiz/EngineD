#include "Precompiled.h"
#include "DepthMapEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void DepthMapEffect::Initialize()
{	 
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}	 
	 
void DepthMapEffect::Terminate()
{	 
	mDepthMapRenderTarget.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}	 
	 
void DepthMapEffect::Begin()
{	 
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);

	mDepthMapRenderTarget.BeginRender();
}	 
	 
void DepthMapEffect::End()
{	 
	mDepthMapRenderTarget.EndRender();
}	 
	 
void DepthMapEffect::Render(const RenderObject& renderObject)
{	 
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera.GetViewMatrix();
	const Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}	 
	 
void DepthMapEffect::DebugUI()
{	 
	if (ImGui::CollapsingHeader("DepthMapEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("DepthMap");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{ 144,144 },
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 1, 1, 1 },
			{ 1, 1, 1, 1 });
		ImGui::DragFloat("Size##DepthMap", &mSize, 1.0f, 1.0f, 1000.0f);
	}
}	 
	 
void DepthMapEffect::SetSize(float size)
{
	mSize = size;
}

void DepthMapEffect::SetCamera(Camera camera)
{
	mCamera = camera;
	mCamera.SetNearPlane(1.0f);
	mCamera.SetFarPlane(25.0f);
}

Camera& EngineD::Graphics::DepthMapEffect::GetCamera()
{
	return mCamera;
}

const Texture& DepthMapEffect::GetDepthMap() const
{
	return mDepthMapRenderTarget;
}
