#include "Precompiled.h"
#include "WaveEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void WaveEffect::Initialize(const std::filesystem::path& filename)
{
	mWaveBuffer.Initialize(mWaves);

	mOceanBuffer.Initialize();
	mTimeBuffer.Initialize();
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<VertexD>(filename);
	mPixelShader.Initialize(filename);
}

void WaveEffect::InitializeWaves(const std::vector<WaveData>& data)
{
	mWaves = data;
	mWaveCount = data.size();
}

void WaveEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
	mTimeBuffer.Terminate();
	mOceanBuffer.Terminate();
	mWaveBuffer.Terminate();
}

void WaveEffect::AddWave(WaveData data)
{
	mWaves.push_back(data);
	++mWaveCount;
	mWaveBuffer.Update(mWaves);
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

	mTimeBuffer.BindVS(1);

	mOceanBuffer.BindVS(2);

	mWaveBuffer.BindVS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;

	Math::Vector3 camPos = mCamera->GetPosition();

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.worldMatrix = matWorld;
	mTransformBuffer.Update(data);

	TimeData timeData;
	timeData.time = mCurrentTime;
	mTimeBuffer.Update(timeData);

	OceanData oceanData;
	oceanData.waveCount = mWaveCount;
	mOceanBuffer.Update(oceanData);

	mWaveBuffer.Update(mWaves);
}

void WaveEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void WaveEffect::End()
{
}

void WaveEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void WaveEffect::DebugUI()
{
	for (int i = 0; i < mWaveCount; ++i)
	{
		ImGui::PushID(i);
		ImGui::CollapsingHeader("Wave");
			ImGui::DragFloat2("Wave Direction", &mWaves[i].direction.x);
			ImGui::DragFloat2("Wave Origin", &mWaves[i].origin.x);
			ImGui::DragFloat("Wave Frequency", &mWaves[i].frequency);
			ImGui::DragFloat("Wave Amplititude", &mWaves[i].amplitude);
			ImGui::DragFloat("Wave Phase", &mWaves[i].phase);
			ImGui::DragFloat("Wave Steepness", &mWaves[i].steepness);
		ImGui::PopID();
	}
}
