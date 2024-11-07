#include "Precompiled.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "PhysicsWorld.h"

using namespace EngineD;
using namespace EngineD::Physics;
using namespace EngineD::Math;
using namespace EngineD::Graphics;

RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr && mMotionState == nullptr, "RigidBody: terminate must be called");
}

void RigidBody::Initialize(EngineD::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
	mGraphicsTransform = &graphicsTransform;
	mMass = mass;

	mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mass, mMotionState, shape.GetCollisionShape());
#ifndef USE_PHYSICS_SERVICE
	PhysicsWorld::Get()->Register(this);
#endif
}

void RigidBody::Terminate()
{
#ifndef USE_PHYSICS_SERVICE
		PhysicsWorld::Get()->Unregister(this);
#endif
	SafeDelete(mRigidBody);
	SafeDelete(mMotionState);
	mGraphicsTransform = nullptr;
}

void RigidBody::SetPosition(const EngineD::Math::Vector3& position)
{
	if (IsDynamic())
	{
		mRigidBody->activate();
	}
	mGraphicsTransform->position = position;
	mRigidBody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));
}

void RigidBody::SetVelocity(const EngineD::Math::Vector3& velocity)
{
	mRigidBody->activate();
	mRigidBody->setLinearVelocity(ConvertTobtVector3(velocity));
}

bool RigidBody::IsDynamic() const
{
	return mMass > 0.0f;
}

void RigidBody::SyncGraphics()
{
	ApplybtTransform(*mGraphicsTransform, mRigidBody->getWorldTransform());
}
