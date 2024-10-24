#include "Precompiled.h"
#include "RenderObjectComponent.h"

void RenderObjectComponent::Initialize()
{
}

void RenderObjectComponent::Terminate()
{
}

void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}
