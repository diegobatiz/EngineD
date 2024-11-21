#pragma once

#include "UIComponent.h"

namespace EngineD
{
	enum class ButtonCallback
	{

	};

	enum class ButtonState
	{
		Default,
		Hover,
		Click,
		Disabled,
		Count
	};

	class UIButtonComponent : public UIComponent
	{
	public:
		SET_TYPE_ID(ComponentId::UIButton);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Render() override;
		void Deserialize(const rapidjson::Value& value) override;

		void SetCallback(ButtonCallback cb);

	private:
		void OnClick();

		ButtonCallback mCallback;
		ButtonState mCurrentState;
		//std::vector<Graphics::UISprite*> mButtonStates;
	};
}