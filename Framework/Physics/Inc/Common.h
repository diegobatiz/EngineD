#pragma once

//Framework Headers
#include <Math/Inc/DMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>

//Bullet Files
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

//Helper Functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

//Math Helper Functions
inline btVector3 ConvertTobtVector3(const EngineD::Math::Vector3& v)
{
	return btVector3(v.x, v.y, v.z);
}

inline btQuaternion ConvertTobtQuaternion(const EngineD::Math::Quaternion& q)
{
	return btQuaternion(q.x, q.y, q.z, q.w);
}

inline btTransform ConvertTobtTransform(const EngineD::Graphics::Transform& transform)
{
	btVector3 v = ConvertTobtVector3(transform.position);
	btQuaternion q = ConvertTobtQuaternion(transform.rotation);
	return btTransform(q, v);
}

inline EngineD::Math::Vector3 ConvertToVector3(const btVector3& v)
{
	return { v.getX(), v.getY(), v.getZ() };
}

inline EngineD::Math::Quaternion ConvertToQuaternion(const btQuaternion& q)
{
	return { q.getX(), q.getY(), q.getZ(), q.getW()};
}

inline void ApplybtTransform(EngineD::Graphics::Transform& transform, const btTransform& t)
{
	transform.position = ConvertToVector3(t.getOrigin());
	transform.rotation = ConvertToQuaternion(t.getRotation());
}