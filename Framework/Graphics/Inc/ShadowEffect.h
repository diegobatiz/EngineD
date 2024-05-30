#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "RenderTarget.h"

namespace EngineD::Graphics
{
	struct RenderObject;

	class ShadowEffect
	{
	public:
		void Initialize();
		void Terminate();
		
		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetFocus(const Math::Vector3& focusPosition);
		void SetSize(float size);

		const Camera& GetLightCamera() const;
		const Texture& GetDepthMap() const;

	private:
		void UpdateLightCamera();

		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		const DirectionalLight* mDirectionalLight = nullptr;

		Math::Vector3 mFocusPosition = Math::Vector3::Zero;
		float mSize = 100.0f;
	};
}