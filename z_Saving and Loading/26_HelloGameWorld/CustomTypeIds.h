#pragma once

#include <EngineD/Inc/EngineD.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<uint32_t>(EngineD::ComponentId::Count)
};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<uint32_t>(EngineD::ServiceId::Count)
};