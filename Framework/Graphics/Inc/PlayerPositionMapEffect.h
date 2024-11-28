#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "RenderTarget.h"
#include "Transform.h"
#include "Camera.h"

namespace EngineD::Graphics
{
	struct RenderObject;

	class PlayerPositionMapEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetPlayerTransform(const Transform& player) { mPlayerTransform = &player; }
		void SetCamera(const Camera& camera) { mCamera = &camera; }

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		struct PlayerPosition
		{
			Math::Vector3 position = Math::Vector3::Zero;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		using PlayerPositionBuffer = TypedStructuredBuffer<PlayerPosition>;
		PlayerPositionBuffer mPositionBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		RenderTarget mPlayerPositionRenderTarget;

		const Transform* mPlayerTransform;
		const Camera* mCamera;
	};
}