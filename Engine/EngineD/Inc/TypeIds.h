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
		Wave,         // Creates a Wave Effect
		Model,        // Creates a model render object
		Animator,     // manages and controls animations
		Rigidbody,	  // physics object that gets added to the physics world
		SoundEffect,  // adds a single sound effect to an object
		SoundBank,     // adds an array of sound effects to an object
		Count
	};

	enum class ServiceId
	{
		Invalid,
		Camera,
		Render,
		Physics,
		Count
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }