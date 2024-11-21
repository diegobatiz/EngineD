#pragma once

#include "UIComponent.h"

namespace EngineD
{
	class UISpriteComponent : public UIComponent
	{
	public:
		SET_TYPE_ID(ComponentId::UISprite);

		void Initialize() override;
		void Terminate() override;
		void Render() override;
		void Deserialize(const rapidjson::Value& value) override;

	private:
		//virtual void ObtainWorldPositon(DirectX::XMFLOAT2& position) override;

		std::filesystem::path mTexturePath;
		DirectX::XMFLOAT2 mPosition;
		Graphics::UISprite mUISprite;
	};
}