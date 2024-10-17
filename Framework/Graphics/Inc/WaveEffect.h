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
		void InitializeWaves(const std::vector<WaveData>& data);
		void Terminate();

		void AddWave(WaveData data);

		void Update(float deltaTime);
		void Begin(); 
		void Render(const RenderObject& renderObject);
		void End();
		void SetCamera(const Camera& camera);
		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 worldMatrix;
		};

		struct TimeData
		{
			float time;
			float padding[3] = { 0.0f };
		};

		struct OceanData
		{
			float waveCount;
			float padding[3] = { 0.0f };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using TimeBuffer = TypedConstantBuffer<TimeData>;
		using OceanBuffer = TypedConstantBuffer<OceanData>;
		using WaveBuffer = TypedStructuredBuffer<WaveData>;

		TransformBuffer mTransformBuffer;
		TimeBuffer mTimeBuffer;
		OceanBuffer mOceanBuffer;
		WaveBuffer mWaveBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		float mCurrentTime;
		int mWaveCount;
		std::vector<WaveData> mWaves;
	};
}