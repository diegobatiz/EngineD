#pragma once

#include "ConstantBuffer.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "RenderTarget.h"
#include "Transform.h"

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

		void SetPlayerTransform(const Transform& player);

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;

		RenderTarget mPlayerPositionRenderTarget;

		const Transform* mPlayerTransform;
	};
}