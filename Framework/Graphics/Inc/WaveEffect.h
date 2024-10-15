#pragma once
#include "ConstantBuffer.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "Colours.h"

namespace EngineD::Graphics
{
	class Camera;
	struct RenderObject;

	class WaveEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

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

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using TimeBuffer = TypedConstantBuffer<TimeData>;

		TransformBuffer mTransformBuffer;
		TimeBuffer mTimeBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		float mCurrentTime;
	};
}