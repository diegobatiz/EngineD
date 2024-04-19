#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& filename)
{
	mTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mVertexShader.Initialize<Vertex>(filename);
	mPixelShader.Initialize(filename);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Standard Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mSettingsBuffer.BindPS(1);
}

void StandardEffect::End()
{
	//nothing until shadows
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(transformData);
	
	SettingsData settingsData;
	settingsData.useDiffuseMap = renderObject.diffuseTextureId > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 1;
	mSettingsBuffer.Update(settingsData);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseTextureId, 0);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
		{
			mSettingsData.useDiffuseMap = useDiffuseMap ? 1 : 0;
		}
	}
}
