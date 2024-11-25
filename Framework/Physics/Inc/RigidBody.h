#pragma once

#include "PhysicsObject.h"

namespace EngineD::Physics
{
	class CollisionShape;

	class RigidBody final : public PhysicsObject
	{
	public:
		RigidBody() = default;
		~RigidBody() override;

		void Initialize(EngineD::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetPosition(const EngineD::Math::Vector3& position);
		void SetVelocity(const EngineD::Math::Vector3& position);

		const EngineD::Math::Vector3 GetVelocity() const;

		bool IsDynamic() const;

	private: 
		void SyncGraphics() override;
		btRigidBody* GetRigidBody() override { return mRigidBody; }

		btRigidBody* mRigidBody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mMass = 0.0f;

		EngineD::Graphics::Transform* mGraphicsTransform = nullptr;
	};
}