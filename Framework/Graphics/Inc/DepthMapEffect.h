#pragma once

#include "Camera.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"
#include "RenderTarget.h"

namespace EngineD::Graphics
{	
	struct RenderObject;

	class DepthMapEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetSize(float size);

		void SetCamera(Camera camera);
		Camera& GetCamera();
		const Texture& GetDepthMap() const;

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		RenderTarget mDepthMapRenderTarget;
		Camera mCamera;

		Math::Vector3 mFocusPosition = Math::Vector3::Zero;
		float mSize = 7.0f;
	};
}