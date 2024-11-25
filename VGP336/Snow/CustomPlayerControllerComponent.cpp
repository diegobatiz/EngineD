#include "CustomPlayerControllerComponent.h"

using namespace EngineD;
using namespace EngineD::Math;
using namespace EngineD::Input;

void CustomPlayerControllerComponent::Initialize()
{
	mRigidBody = GetOwner().GetComponent<RigidBodyComponent>();
}

void CustomPlayerControllerComponent::Terminate()
{
}

void CustomPlayerControllerComponent::Update(float deltaTime)
{
	CalculateDirection();

	mRBVelocity = mRigidBody->GetVelocity();

	Vector3 targetVelocity = { mDirection.x * mMaxSpeed, 0.0f, mDirection.z * mMaxSpeed };

	mRBVelocity = Vector3::SmoothDamp(mRBVelocity, targetVelocity, mCurrentVelocity, mMoveSmoothing, 1000.0f, deltaTime);
	
	mRigidBody->SetVelocity(mRBVelocity);
}

void CustomPlayerControllerComponent::DebugUI()
{
	if (ImGui::CollapsingHeader("Player Controller"))
	{
		ImGui::DragFloat("Max Speed", &mMaxSpeed, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Smooth Time", &mMoveSmoothing, 0.01f, 0.001f, 5.0f);
	}
}

void CustomPlayerControllerComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MaxSpeed"))
	{
		mMaxSpeed = value["MaxSpeed"].GetFloat();
	}

	if (value.HasMember("SmoothTime"))
	{
		mMoveSmoothing = value["SmoothTime"].GetFloat();
	}
}

void CustomPlayerControllerComponent::CalculateDirection()
{
	auto input = Input::InputSystem::Get();
	mDirection = Vector3::Zero;
	if (input->IsKeyDown(KeyCode::W))
	{
		mDirection.z = 1.0f;
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mDirection.z = -1.0f;
	}

	if (input->IsKeyDown(KeyCode::A))
	{
		mDirection.x = -1.0f;
	}
	else if (input->IsKeyDown(KeyCode::D))
	{
		mDirection.x = 1.0f;
	}

	if (MagnitudeSqr(mDirection))
	{
		mDirection = Normalize(mDirection);
	}
}
