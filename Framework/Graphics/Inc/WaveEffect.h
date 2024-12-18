#pragma once
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "Colours.h"

namespace EngineD::Graphics
{
	class Camera;
	struct RenderObject;
	struct DirectionalLight;

	//Used in Sum of Sines version of effect
	struct WaveData
	{
		Math::Vector2 direction;
		Math::Vector2 origin;
		float frequency;
		float amplitude;
		float phase;
		float steepness;
	};

	class WaveEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void InitializeWaves(std::vector<WaveData> data);
		void Terminate();

		void AddWave(WaveData data);

		void Update(float deltaTime);
		void Begin(); 
		void Render(const RenderObject& renderObject);
		void End();
		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& light);
		void DebugUI();

		struct OceanData
		{
			int waveCount;
			int pixelWaveCount;
			float vertexFrequency = 1;
			float vertexAmplitude = 1;
			float vertexInitialSpeed = 1;
			float vertexSeed = 0.6;
			float vertexMaxPeak = 2;
			float vertexPeakOffset = 0.3f;
			float vertexFrequencyMult = 1.3f;
			float vertexAmplitudeMult = 0.3f;
			float vertexSpeedRamp = 1.6f;
			float vertexSeedIter = 1.4326f;
			float vertexHeight = 1;
			float vertexDrag = 1;
			float padding[2] = { 0.0f };
		};

		struct LightData
		{
			float normalStrength = 0.5f;
			Math::Vector3 diffuseReflectance = { 1.0f, 1.0f, 2.0f };
			float specNormalStrength = 0.5f;
			Math::Vector3 specularReflectance = { 0.5f, 0.5f, 0.5f };
			float shininess = 1.0f;
			Math::Vector3 ambientColor = { 0.1f, 0.1f, 0.2f };
			Math::Vector4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			float tipAttenuation = 0.5f;
			Math::Vector3 tipColor = { 0.5f, 0.5f, 0.5f };
			Math::Vector4 diffuseColor = { 0.0f, 0.0f, 1.0f, 1.0f };
			float fresnelNormalStrength = 1.0f;
			float fresnelShininess = 1.0f;
			float fresnelBias = 1.0f;
			float fresnelStrength = 1.0f;
			Math::Vector4 fresnelColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		};


		void SetWaveData(const OceanData& data, const LightData& lightData);
		OceanData GetOceanData() { return mOceanData; }
		LightData GetLightData() { return m_LightData; }

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 worldMatrix;
			Math::Vector3 lightDirection;
			float padding1;
			Math::Vector3 cameraPos;
			float padding2;
		};

		struct TimeData
		{
			float time;
			float padding[3] = { 0.0f };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightBuffer = TypedConstantBuffer<LightData>;
		using TimeBuffer = TypedConstantBuffer<TimeData>;
		using OceanBuffer = TypedConstantBuffer<OceanData>;
		using WaveBuffer = TypedStructuredBuffer<WaveData>;

		TransformBuffer mTransformBuffer;
		LightBuffer m_LightBuffer;
		TimeBuffer mTimeBuffer;
		OceanBuffer mOceanBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		const DirectionalLight* m_DirectionalLight = nullptr;

		LightData m_LightData;
		OceanData mOceanData;

		float mCurrentTime = 0.0f;
	};
}