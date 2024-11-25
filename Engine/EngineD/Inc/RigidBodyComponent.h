#pragma once

#include "Component.h"

namespace EngineD
{
	class RigidBodyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Rigidbody);

		void Initialize() override;
		void Terminate() override;
		void Deserialize(const rapidjson::Value& value) override;

		void SetPosition(const Math::Vector3& position);
		void SetVelocity(const Math::Vector3& velocity);

		const Math::Vector3 GetVelocity() const { return mRigidBody.GetVelocity(); }

	private:
		friend class PhysicsService;

		Physics::CollisionShape mCollisionShape;
		Physics::RigidBody mRigidBody;
		float mMass = -1.0f;
	};
}