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
	Graphics_D3D11* gs = GraphicsSystem::Get();
	gs->ResetRenderTarget();
	gs->ResetViewport();
}

void ComicBookEffect::Render(const RenderObject& renderObject)
{
	Graphics_D3D11* gs = GraphicsSystem::Get();

	mSobelRenderTexture.BeginRender();
		mSourceTexture->BindPS(0);
		mPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
	mSobelRenderTexture.EndRender();

	/*for (uint32_t i = 1; i < mBlurIterations; i++)
	{
		mVerticalLineRenderTexture.BeginRender();
			mHorizontalLineRenderTexture.BindPS(0);
			mVerticalLinesPS.Bind();
			renderObject.meshBuffer.Render();
			Texture::UnbindPS(0);
		mVerticalLineRenderTexture.EndRender();

		mHorizontalLineRenderTexture.BeginRender();
			mVerticalLineRenderTexture.BindPS(0);
			mHorizontalLinesPS.Bind();
			renderObject.meshBuffer.Render();
			Texture::UnbindPS(0);
		mHorizontalLineRenderTexture.EndRender();
	}

	mHorizontalLineRenderTexture.BeginRender();
		mVerticalLineRenderTexture.BindPS(0);
		mHorizontalLinesPS.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
	mHorizontalLineRenderTexture.EndRender();
	*/
}

void ComicBookEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("GaussianBlurEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("DrawIterations", &mDrawIterations, 1, 1, 100);
	}
}

void ComicBookEffect::SetSourceTexture(const Texture& texture)
{
	mSourceTexture = &texture;
}

const Texture& ComicBookEffect::GetResultTexture() const
{
	return mSobelRenderTexture;
}