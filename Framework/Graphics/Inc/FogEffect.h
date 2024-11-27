#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"

namespace EngineD::Graphics
{
	class Camera;
	struct RenderObject;

	class FogEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();
		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);
		void DebugUI();

		void SetFogColor(Math::Vector4 fogColour) { mColorData.fogColour = fogColour; }

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Vector3 cameraPos;
			float padding;
		};

		struct ColorData
		{
			Math::Vector4 fogColour;
			float fogDensity;
			float fogOffset;
			float padding[2] = { 0.0f };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using ColorBuffer = TypedConstantBuffer<ColorData>;

		TransformBuffer mTransformBuffer;
		ColorBuffer mColorBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		const Camera* mCamera = nullptr;
		ColorData mColorData;
	};
}