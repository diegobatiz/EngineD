#include "Precompiled.h"
#include "PhysicsWorld.h"

#include "PhysicsObject.h"

using namespace EngineD;
using namespace EngineD::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: is not initialized");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: terminate must be called");
}

void PhysicsWorld::Initialize(const Settings& settings)
{

}

void PhysicsWorld::Terminate()
{
}

void PhysicsWorld::Update(float deltaTime)
{
}

void PhysicsWorld::DebugUI()
{
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
}
