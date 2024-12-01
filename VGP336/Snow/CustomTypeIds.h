#pragma once

#include <EngineD/Inc/EngineD.h>

enum class CustomComponentId
{
	CustomPlayerController = static_cast<uint32_t>(EngineD::ComponentId::Count)
};

enum class CustomServiceId
{
	SnowRenderService = static_cast<uint32_t>(EngineD::ServiceId::Count)
};