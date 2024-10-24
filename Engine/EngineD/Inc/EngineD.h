#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"

#include "Event.h"
#include "EventManager.h"

#include "SaveUtil.h"
#include "GameObjectFactory.h"

//Game World
#include "GameWorld.h"

//Services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"

//GameObject
#include "GameObject.h"

//Components
#include "TypeIds.h"
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "RenderObjectComponent.h"
#include "TransformComponent.h"

namespace EngineD
{
	App& MainApp();
}