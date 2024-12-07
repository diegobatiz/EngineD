#pragma once

#include "CustomTypeIds.h"

class CustomPlayerControllerComponent : public EngineD::Component
{
public:
	SET_TYPE_ID(CustomComponentId::CustomPlayerController);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;

	void DebugUI() override;

	void Deserialize(const rapidjson::Value& value) override;

private:
	void CalculateDirection();

	EngineD::Math::Vector3 mDirection;
	EngineD::Math::Vector3 mRBVelocity;
	EngineD::Math::Vector3 mCurrentVelocity;
	float mMaxSpeed;
	float mMoveSmoothing;
	EngineD::RigidBodyComponent* mRigidBody;
};