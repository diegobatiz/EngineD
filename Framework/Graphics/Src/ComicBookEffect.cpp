#include "Precompiled.h"
#include "ComicBookEffect.h"

#include "GraphicsSystem.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void ComicBookEffect::Initialize(const std::filesystem::path& filename)
{
	Graphics_D3D11* gs = GraphicsSystem::Get();
	const float screenWidth = gs->GetBackBufferWidth();
	const float screenHeight = gs->GetBackBufferHeight();
	mSobelRenderTexture.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mVertexShader.Initialize<VertexPX>(filename);
	mPixelShader.Initialize(filename);
}

void ComicBookEffect::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mSobelRenderTexture.Terminate();
}

void ComicBookEffect::Begin()
{
	mVertexShader.Bind();
	mNormalTexture->BindPS(0);
	mDepthTexture->BindPS(1);
	mPixelShader.Bind();

	Graphics_D3D11* gs = GraphicsSystem::Get();
	SettingsData data;
	data.screenWidth = gs->GetBackBufferWidth();
	data.screenHeight = gs->GetBackBufferHeight();
	mSettingsBuffer.Update(data);
	mSettingsBuffer.BindPS(0);

	mSampler.BindPS(0);
}

void ComicBookEffect::End()
{
	Texture::UnbindPS(0);
	Texture::UnbindPS(1);
}

void ComicBookEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void ComicBookEffect::DebugUI()
{
	
}

void ComicBookEffect::SetNormalTexture(const Texture& texture)
{
	mNormalTexture = &texture;
}

void ComicBookEffect::SetDepthTexture(const Texture& texture)
{
	mDepthTexture = &texture;
}

const Texture& ComicBookEffect::GetResultTexture() const
{
	return mSobelRenderTexture;
}