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

		struct LightData
		{
			float normalStrength = 0.5f;
			Math::Vector3 diffuseReflectance = { 1.0f, 1.0f, 2.0f};
			float specNormalStrength = 0.5f;
			Math::Vector3 specularReflectance = { 0.5f, 0.5f, 0.5f};
			float shininess = 1.0f;
			Math::Vector3 ambientColor = { 0.1f, 0.1f, 0.2f};
			Math::Vector4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		};

		struct TimeData
		{
			float time;
			float padding[3] = { 0.0f };
		};

		struct OceanData
		{
			int waveCount;
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
		WaveBuffer mWaveBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		const DirectionalLight* m_DirectionalLight = nullptr;

		LightData m_LightData;

		float mCurrentTime = 0.0f;
		int mWaveCount = 0;
		std::vector<WaveData> mWaves;
	};
}