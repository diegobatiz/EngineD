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
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mVertexShader.Initialize<Vertex>(filename);
	mPixelShader.Initialize(filename);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Standard Effect: no camera set!");
	ASSERT(mDirectionalLight != nullptr, "Standard Effect: no light set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);

	mSettingsBuffer.BindVS(1);
	mSettingsBuffer.BindPS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindPS(3);
}

void StandardEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	SettingsData settingsData;
	settingsData.useDiffuseMap = renderObject.diffuseMapId > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 0;
	settingsData.useNormalMap = renderObject.normalMapId > 0 && mSettingsData.useNormalMap > 0 ? 1 : 0;
	settingsData.useSpecMap = renderObject.specMapId > 0 && mSettingsData.useSpecMap > 0 ? 1 : 0;
	settingsData.useLighting = mSettingsData.useLighting > 0 ? 1 : 0;
	settingsData.useBumpMap = renderObject.bumpMapId > 0 && mSettingsData.useBumpMap > 0;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	settingsData.useShadowMap = mShadowMap != nullptr && mSettingsData.useShadowMap > 0;
	settingsData.depthBias = mSettingsData.depthBias;
	mSettingsBuffer.Update(settingsData);

	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Math::Transpose(matFinal);
	transformData.world = Math::Transpose(matWorld);
	transformData.viewPosition = mCamera->GetPosition();

	if (settingsData.useShadowMap > 0)
	{
		const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
		const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = Math::Transpose(matWorld * matLightView * matLightProj);

		mShadowMap->BindPS(4);
	}
	mTransformBuffer.Update(transformData);

	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindPS(renderObject.specMapId, 2);
	tm->BindVS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
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

		bool useNormalMap = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
		{
			mSettingsData.useNormalMap = useNormalMap ? 1 : 0;
		}

		bool useSpecMap = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecMap", &useSpecMap))
		{
			mSettingsData.useSpecMap = useSpecMap ? 1 : 0;
		}

		bool useLighting = mSettingsData.useLighting > 0;
		if (ImGui::Checkbox("UseLighting", &useLighting))
		{
			mSettingsData.useLighting = useLighting ? 1 : 0;
		}

		bool useBumpMap = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
		{
			mSettingsData.useBumpMap = useBumpMap ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 5.0f);

		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("useShadowMap", &useShadowMap))
		{
			mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
	}
}
