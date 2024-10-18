#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"

#include "Event.h"
#include "EventManager.h"

//Game World
#include "GameWorld.h"

//Services
#include "Service.h"

//GameObject
#include "GameObject.h"

//Components
#include "TypeIds.h"
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"

namespace EngineD
{
	App& MainApp();
}