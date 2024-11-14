#pragma once

namespace EngineD::Graphics
{
	class UISprite;

	class UISpriteRenderer final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static UISpriteRenderer* Get();

		UISpriteRenderer() = default;
		~UISpriteRenderer();

		void Initialize();
		void Terminate();

		void BeginRender();
		void EndRender();

		void Render(const UISprite* sprite);

	private:
		DirectX::CommonStates* mCommonStates = nullptr;
		DirectX::SpriteBatch* mSpriteBatch = nullptr;
		DirectX::XMMATRIX mTransform = DirectX::XMMatrixIdentity();
	};
}