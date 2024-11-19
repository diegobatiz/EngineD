#include "Precompiled.h"
#include "AtmosphereEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void AtmosphereEffect::Initialize()
{
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Atmosphere.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void AtmosphereEffect::Terminate()
{
	mSampler.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void AtmosphereEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
}

void AtmosphereEffect::End()
{

}

void AtmosphereEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);

	renderObject.meshBuffer.Render();
}

void AtmosphereEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("NormalMapEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("NormalMap");
	}
}

void AtmosphereEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}