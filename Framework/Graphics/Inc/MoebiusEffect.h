#pragma once

#include "ConstantBuffer.h"
#include "Sampler.h"
#include "VertexShader_D3D11.h"
#include "PixelShader_D3D11.h"

namespace EngineD::Graphics
{
	class Texture;
	struct RenderObject;

	class MoebiusEffect
	{
	public:
		void Initialize(const std::filesystem::path& filename);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetTexture(const Texture* texture, uint32_t slot = 0);

	private:
		Sampler mSampler;
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		std::array<const Texture*, 4> mTextures;
	};
}