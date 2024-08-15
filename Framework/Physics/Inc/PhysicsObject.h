#pragma once


namespace EngineD::Physics
{
	class PhysicsObject
	{
		PhysicsObject() = default;
		virtual ~PhysicsObject() = default;

	protected:
		friend class PhysicsWorld;
		virtual void SyncGraphics() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
	};
}