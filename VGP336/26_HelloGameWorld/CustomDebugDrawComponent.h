#pragma once

#include "CustomTypeIds.h"

class CustomDebugDrawComponent : public EngineD::Component
{
public:
	SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Deserialize(const rapidjson::Value& value) override;

	void AddDebugDraw();

private:
	const EngineD::TransformComponent* mTransformComponent = nullptr;
	EngineD::Math::Vector3 mPosition = EngineD::Math::Vector3::Zero;
	EngineD::Color mColor = EngineD::Colors::White;
	uint32_t mSlices = 0;
	uint32_t mRings = 0;
	float mRadius = 0.0f;
};