#include "Precompiled.h"
#include "LookCameraComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

using namespace EngineD;
using namespace EngineD::Input;
using namespace EngineD::Graphics;

void LookCameraComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	ASSERT(mCameraComponent != nullptr, "LookCameraComponent: Camera not found");
}

void LookCameraComponent::Terminate()
{
	mCameraComponent = nullptr;
}

void LookCameraComponent::Update(float deltaTime)
{
	Camera& camera = mCameraComponent->GetCamera();

	auto input = InputSystem::Get(); 
	const float turnSpeed = mTurnSpeed;

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void LookCameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveFloat("TurnSpeed", mTurnSpeed, doc, componentValue);
	value.AddMember("LookCameraComponent", componentValue, doc.GetAllocator());
}

void LookCameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("TurnSpeed"))
	{
		mTurnSpeed = value["TurnSpeed"].GetFloat();
	}
	if (value.HasMember("CanLook"))
	{
		mCanLook = value["CanLook"].GetBool();
	}
}
