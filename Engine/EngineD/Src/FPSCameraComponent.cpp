#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

void FPSCameraComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: Camera not found");
}

void FPSCameraComponent::Terminate()
{
	mCameraComponent = nullptr;
}

void FPSCameraComponent::Update(float deltaTime)
{
	Camera& camera = mCameraComponent->GetCamera();

	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? mShiftSpeed : mMoveSpeed;
	const float turnSpeed = mTurnSpeed;

	if (input->IsKeyDown(KeyCode::W))
	{
		camera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		camera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		camera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void FPSCameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveFloat("MoveSpeed", mMoveSpeed, doc, componentValue);
	SaveUtil::SaveFloat("ShiftSpeed", mShiftSpeed, doc, componentValue);
	SaveUtil::SaveFloat("TurnSpeed", mTurnSpeed, doc, componentValue);
	value.AddMember("FPSCameraComponent", componentValue, doc.GetAllocator());
}

void FPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}
	if (value.HasMember("ShiftSpeed"))
	{
		mShiftSpeed = value["ShiftSpeed"].GetFloat();
	}
	if (value.HasMember("TurnSpeed"))
	{
		mTurnSpeed = value["TurnSpeed"].GetFloat();
	}
}
