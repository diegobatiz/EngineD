#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace EngineD;
using namespace EngineD::Physics;

void PhysicsService::Update(float deltaTime)
{
	if (mEnabled)
	{
		PhysicsWorld::Get()->Update(deltaTime);
	}
}

void PhysicsService::DebugUI()
{
	if (mEnabled)
	{
		PhysicsWorld::Get()->DebugUI();
	}
}

void PhysicsService::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
	rapidjson::Value serviceValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Gravity", settings.gravity, doc, serviceValue);
	SaveUtil::SaveFloat("SimSteps", settings.simulationSteps, doc, serviceValue);
	SaveUtil::SaveFloat("FixedTimeStep", settings.fixedTimeStep, doc, serviceValue);
	value.AddMember("PhysicsService", serviceValue, doc.GetAllocator());
}

void PhysicsService::Deserialize(const rapidjson::Value& value)
{
	PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
	if (value.HasMember("Gravity"))
	{
		const auto& gravity = value["Gravity"].GetArray();
		settings.gravity.x = gravity[0].GetFloat();
		settings.gravity.y = gravity[1].GetFloat();
		settings.gravity.z = gravity[2].GetFloat();
	}
	if (value.HasMember("SimSteps"))
	{
		settings.simulationSteps = value["SimSteps"].GetFloat();
	}
	if (value.HasMember("FixedTimeStep"))
	{
		settings.fixedTimeStep = value["FixedTimeStep"].GetFloat();
	}
	PhysicsWorld::Get()->UpdateSettings(settings);
}

void PhysicsService::Register(RigidBodyComponent* rbc)
{
	PhysicsWorld::Get()->Register(&rbc->mRigidBody);
}

void PhysicsService::Unregister(RigidBodyComponent* rbc)
{
	PhysicsWorld::Get()->Unregister(&rbc->mRigidBody);
}

void PhysicsService::SetEnable(bool enabled)
{
	mEnabled = enabled;
}
