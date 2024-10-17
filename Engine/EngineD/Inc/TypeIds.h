#pragma once

namespace EngineD
{
	enum class ComponentId
	{
		Invalid,      // default value
		Transform,    // transform component for location data
		Count
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }