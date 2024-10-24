#include "Precompiled.h"
#include "WaveEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"
#include "LightTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void WaveEffect::Initialize(const std::filesystem::path& filename)
{
	mWaveBuffer.Initialize(mWaves);

	mOceanBuffer.Initialize();
	mTimeBuffer.Initialize();
	m_LightBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<VertexPC>(filename);
	mPixelShader.Initialize(filename);
}

void WaveEffect::InitializeWaves(std::vector<WaveData> data)
{
	mWaves = data;
	mWaveCount = data.size();
}

void WaveEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	m_LightBuffer.Terminate();
	mTimeBuffer.Terminate();
	mOceanBuffer.Terminate();
	mWaveBuffer.Terminate();
}

void WaveEffect::AddWave(WaveData data)
{
	mWaves.push_back(data);
	++mWaveCount;
}

void WaveEffect::Update(float deltaTime)
{
	mCurrentTime += deltaTime;
}

void WaveEffect::Begin()
{
	ASSERT(mCamera != nullptr, "Grass Effect: no camera set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	m_LightBuffer.BindPS(1);

	mTimeBuffer.BindVS(2);

	mOceanBuffer.BindVS(3);
	mOceanBuffer.BindPS(3);

	mWaveBuffer.BindVS(0);
	mWaveBuffer.BindPS(0);
}

void WaveEffect::Render(const RenderObject& renderObject)
{
	Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	Math::Vector3 camPos = mCamera->GetPosition();

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.worldMatrix = Math::Transpose(matWorld);
	data.lightDirection = m_DirectionalLight->direction;
	data.cameraPos = camPos;
	mTransformBuffer.Update(data);

	LightData lightData;
	lightData.normalStrength = m_LightData.normalStrength;
	lightData.specNormalStrength = m_LightData.specNormalStrength;
	lightData.diffuseReflectance = m_LightData.diffuseReflectance;
	lightData.specularReflectance = m_LightData.specularReflectance;
	lightData.shininess = m_LightData.shininess;
	lightData.ambientColor = m_LightData.ambientColor;
	m_LightBuffer.Update(lightData);

	TimeData timeData;
	timeData.time = mCurrentTime;
	mTimeBuffer.Update(timeData);

	OceanData oceanData;
	oceanData.waveCount = mWaveCount;
	mOceanBuffer.Update(oceanData);

	mWaveBuffer.Update(mWaves.data());

	renderObject.meshBuffer.Render();
}

void WaveEffect::End()
{
}

void WaveEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void WaveEffect::SetDirectionalLight(const DirectionalLight& light)
{
	m_DirectionalLight = &light;
}

void WaveEffect::DebugUI()
{
	for (int i = 0; i < mWaveCount; ++i)
	{
		ImGui::PushID(i);
		ImGui::CollapsingHeader("Wave");
			ImGui::DragFloat2("Wave Direction", &mWaves[i].direction.x, 0.01f);
			ImGui::DragFloat2("Wave Origin", &mWaves[i].origin.x);
			ImGui::DragFloat("Wave Frequency", &mWaves[i].frequency, 0.01f);
			ImGui::DragFloat("Wave Amplititude", &mWaves[i].amplitude, 0.01f);
			ImGui::DragFloat("Wave Phase", &mWaves[i].phase, 0.01f);
			ImGui::DragFloat("Wave Steepness", &mWaves[i].steepness, 0.01f);
		ImGui::PopID();
	}

	ImGui::DragFloat3("Diffuse Reflectance", &m_LightData.diffuseReflectance.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Specular Reflectance", &m_LightData.specularReflectance.x, 0.01f, 0.0f);
	ImGui::ColorEdit3("Ambient Color", &m_LightData.ambientColor.x, 0.001f);
	ImGui::ColorEdit4("Specular Color", &m_LightData.specularColor.x, 0.001f);
	ImGui::DragFloat("Normal Strength", &m_LightData.normalStrength, 0.01f);
	ImGui::DragFloat("Spec Normal Strength", &m_LightData.specNormalStrength, 0.01f);
	ImGui::DragFloat("Shininess", &m_LightData.shininess, 1.0f, 0.01f);
}
