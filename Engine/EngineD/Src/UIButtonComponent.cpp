#include "Precompiled.h"
#include "UIButtonComponent.h"

#include "UIRenderService.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void UIButtonComponent::Initialize()
{
	for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
	{

	}

	UIRenderService* renderService = GetOwner().GetWorld().GetService<UIRenderService>();
}

void UIButtonComponent::Terminate()
{
}

void UIButtonComponent::Update(float deltaTime)
{
	mCurrentState = ButtonState::Default;

	InputSystem* input = InputSystem::Get();
	const int mouseX = input->GetMouseScreenX();
	const int mouseY = input->GetMouseScreenY();
	if (mButtonStates[0].IsInSprite(mouseX, mouseY))
	{

	}
}

void UIButtonComponent::Render()
{
	uint32_t buttonState = static_cast<uint32_t>(mCurrentState);
	if (mButtonStateTextures[buttonState].empty())
	{
		buttonState = 0;
	}
	UISpriteRenderer::Get()->Render(&mButtonStates[buttonState]);
}

void UIButtonComponent::Deserialize(const rapidjson::Value& value)
{
	const uint32_t buttonCount = static_cast<uint32_t>(ButtonState::Count);

	if (value.HasMember("Position"))
	{
		auto pos = value["Position"].GetArray();
		for (uint32_t i = 0; i < buttonCount; ++i)
		{

		}
	}
	if (value.HasMember("Rotation"))
	{
		auto pos = value["Rotation"].GetArray();
		for (uint32_t i = 0; i < buttonCount; ++i)
		{

		}
	}

	for (uint32_t i = 0; i < buttonCount; ++i)
	{
		std::string buttonStateStr = "";
		ButtonState state = (ButtonState)i;
		switch (state)
		{
		case ButtonState::Default:	buttonStateStr = "Default"; break;
		case ButtonState::Hover:	buttonStateStr = "Hover"; break;
		case ButtonState::Click:	buttonStateStr = "Click"; break;
		case ButtonState::Disabled: buttonStateStr = "Disabled"; break;
		default: break;
		}

		if (!value.HasMember(buttonStateStr.c_str()))
		{
			continue;
		}

		auto buttonStateObj = value[buttonStateStr.c_str()].GetObj();
		if (buttonStateObj.HasMember("Texture"))
		{
			mButtonStateTextures[i] = buttonStateObj["Texture"].GetString();
		}

		if (buttonStateObj.HasMember("Flip"))
		{
			std::string flip = buttonStateObj["Flip"].GetString();
		}
	}
}

void UIButtonComponent::SetCallback(ButtonCallback cb)
{
	mCallback = cb;
}

void UIButtonComponent::OnClick()
{
}
