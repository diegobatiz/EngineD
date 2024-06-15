#include "Precompiled.h"
#include "MoebiusEffect.h"
#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;


void MoebiusEffect::Initialize(const std::filesystem::path& filename)
{
	mVertexShader.Initialize<VertexPX>(filename);
	mPixelShader.Initialize(filename);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void MoebiusEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void MoebiusEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			mTextures[i]->BindPS(i);
		}
	}
}

void MoebiusEffect::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			Texture::UnbindPS(i);
		}
	}
}

void MoebiusEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void MoebiusEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("MoebiusEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
	}
}

void MoebiusEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "PostProcessingEffect: invalid slot index");
	mTextures[slot] = texture;
}
