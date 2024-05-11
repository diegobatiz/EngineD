#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"

namespace EngineD::Graphics
{
	class Texture;
	struct RenderObject;

	class PostProcessingEffect
	{
	public:
		enum class Mode
		{
			None,
			Monochrome,
			Invert,
			Mirror,
			Blur,
			Combine2,
			MotionBlur,
			ChromaticAberration
		};

		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetTexture(const Texture* texture, uint32_t slot = 0);

	private:
		struct PostProcessData
		{
			int mode = 0;
			float params0 = 0.0f;
			float params1 = 0.0f;
			float params2 = 0.0f;
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessBuffer mPostProcessingBuffer;

		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		std::array<const Texture*, 4> mTextures;

		Mode mMode = Mode::None;
		float mMirrorX = -1.0f;
		float mMirrorY = 1.0f;
		float mBlurStrength = 5.0f;
		float mAberrationValue;
	};
}