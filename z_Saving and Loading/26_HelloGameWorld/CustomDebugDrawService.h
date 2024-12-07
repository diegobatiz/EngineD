#pragma once

#include "CustomTypeIds.h"

class CustomDebugDrawComponent;

class CustomDebugDrawService : public EngineD::Service
{
public:
	SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

	void Render() override;
	void DebugUI() override;

	void Register(CustomDebugDrawComponent* rbc);
	void Unregister(CustomDebugDrawComponent* rbc);

private:
	using CustomDebugDrawComponents = std::vector<CustomDebugDrawComponent*>;
	CustomDebugDrawComponents mCustomDebugDrawComponents;
	bool mIsEnabled;
};