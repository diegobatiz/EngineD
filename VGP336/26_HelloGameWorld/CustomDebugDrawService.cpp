#include "CustomDebugDrawService.h"

#include "CustomDebugDrawComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Math;

void CustomDebugDrawService::Render()
{
	for (CustomDebugDrawComponent* component : mCustomDebugDrawComponents)
	{
		component->AddDebugDraw();
	}
}

void CustomDebugDrawService::Register(CustomDebugDrawComponent* component)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), component);
	if (iter == mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.push_back(component);
	}
}

void CustomDebugDrawService::Unregister(CustomDebugDrawComponent* component)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), component);
	if (iter != mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.erase(iter);
	}
}
