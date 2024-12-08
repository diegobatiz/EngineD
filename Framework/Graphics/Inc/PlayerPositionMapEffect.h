#pragma once

#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "Sampler.h"
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
		void SetRadius(float radius);
		void SetSnowDimensions(float width, float height);

		const Texture& GetPositionMap() const;

	private:
		struct PlayerPosition
		{
			Math::Vector2 position = Math::Vector2::Zero;
			float playerRadius = 10;
			float padding;
		};

		struct TrailSettings
		{
			float startGradient = 0.5f;
			float edgeThickness = 0.1f;
			float padding[2];
		};

		using PlayerPositionBuffer = TypedConstantBuffer<PlayerPosition>;
		using TrailBuffer = TypedConstantBuffer<TrailSettings>;
		PlayerPositionBuffer mPositionBuffer;
		TrailBuffer mTrailBuffer;

		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		Sampler mSampler;

		RenderTarget mPlayerPositionRenderTargetA;
		RenderTarget mPlayerPositionRenderTargetB;

		const Transform* mPlayerTransform;

		float mRadius;
		float mSnowHeight;
		float mSnowWidth;

		TrailSettings mTrailSettings;

		bool mUseA = true;
	};
}