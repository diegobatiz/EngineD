#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"

namespace EngineD::Graphics
{
	class Camera;
	struct RenderObject;

	class StandardEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);

		void DebugUI();

	private: 
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			float padding[3] = { 0 };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		SettingsBuffer mSettingsBuffer;

		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
	};
}