#pragma once

namespace EngineD::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeSphere(float radius);
		void InitializeBox(const EngineD::Math::Vector3& halfExtents);
		void InitializeHull(const EngineD::Math::Vector3&, const EngineD::Math::Vector3& origin);
		void InitializeEmpty();
		void Terminate();

	private:
		btCollisionShape* mCollisionShape = nullptr;

		friend class RigidBody;
		btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
	};
}