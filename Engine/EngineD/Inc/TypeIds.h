#pragma once

namespace EngineD
{
	enum class ComponentId
	{
		Invalid,      // default value
		Transform,    // transform component for location data
		Camera,       // contains a Camera for viewing
		FPSCamera,    // moves Camera with FPS controls
		Mesh,         // Creates a mesh render object
		Count
	};

	enum class ServiceId
	{
		Invalid,
		Camera,
		Render,
		Count
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }