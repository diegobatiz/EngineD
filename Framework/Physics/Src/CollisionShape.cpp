#include "Precompiled.h"
#include "CollisionShape.h"

using namespace EngineD;
using namespace EngineD::Math;
using namespace EngineD::Physics;

CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: terminate must be called");
}

void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btSphereShape(radius);
}

void CollisionShape::InitializeBox(const EngineD::Math::Vector3& halfExtents)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btBoxShape(ConvertTobtVector3(halfExtents));
}

void CollisionShape::InitializeHull(const EngineD::Math::Vector3& halfExtents, const EngineD::Math::Vector3& origin)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	btConvexHullShape* hullShape = new btConvexHullShape();
	std::vector<Vector3> points =
	{
		{-halfExtents.x, -halfExtents.y, -halfExtents.z},
		{-halfExtents.x,  halfExtents.y, -halfExtents.z},
		{ halfExtents.x,  halfExtents.y, -halfExtents.z},
		{ halfExtents.x, -halfExtents.y, -halfExtents.z},
		{-halfExtents.x, -halfExtents.y,  halfExtents.z},
		{-halfExtents.x,  halfExtents.y,  halfExtents.z},
		{ halfExtents.x,  halfExtents.y,  halfExtents.z},
		{ halfExtents.x, -halfExtents.y,  halfExtents.z}
	};
	for (Vector3& point : points)
	{
		hullShape->addPoint(ConvertTobtVector3(point + origin), false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;
}

void CollisionShape::InitializeEmpty()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: is already initialized");
	mCollisionShape = new btEmptyShape();
}

void CollisionShape::Terminate()
{
	SafeDelete(mCollisionShape);
}
