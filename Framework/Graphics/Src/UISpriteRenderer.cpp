#include "Precompiled.h"
#include "UISpriteRenderer.h"
#include "UISprite.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

namespace
{
	std::unique_ptr<UISpriteRenderer> sSpriteRenderer;
}

void UISpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: is already initialized");
	sSpriteRenderer = std::make_unique<UISpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void UISpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

UISpriteRenderer* EngineD::Graphics::UISpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "UISpriteRenderer: is not initialized");
	return sSpriteRenderer.get();
}

UISpriteRenderer::~UISpriteRenderer()
{
	ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: Terminate must be called");
}

void UISpriteRenderer::Initialize()
{
	Graphics_D3D11* gs = GraphicsSystem::Get();
	mCommonStates = new DirectX::CommonStates(gs->GetDevice());
	mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

void UISpriteRenderer::Terminate()
{
	if (mCommonStates != nullptr)
	{
		delete mCommonStates;
		mCommonStates = nullptr;
	}

	if (mSpriteBatch != nullptr)
	{
		delete mSpriteBatch;
		mSpriteBatch = nullptr;
	}
}

void UISpriteRenderer::BeginRender()
{
	mSpriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		mCommonStates->NonPremultiplied(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		mTransform);
}

void UISpriteRenderer::EndRender()
{
	mSpriteBatch->End();

	//restore the state object
	auto blendState = mCommonStates->Opaque();
	auto depthStencilState = mCommonStates->DepthDefault();
	auto rasterizerState = mCommonStates->CullCounterClockwise();

	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState, 0);
	context->RSSetState(rasterizerState);
}

void UISpriteRenderer::Render(const UISprite* sprite)
{
	const Texture* texture = TextureManager::Get()->GetTexture(sprite->mTextureId);
	if (texture != nullptr)
	{
		mSpriteBatch->Draw(
			(ID3D11ShaderResourceView*)texture->GetRawData(),
			sprite->mPosition,
			&sprite->mRect,
			sprite->mColor,
			sprite->mRotation,
			sprite->mOrigin,
			sprite->mScale,
			sprite->mFlip
		);
	}
}
