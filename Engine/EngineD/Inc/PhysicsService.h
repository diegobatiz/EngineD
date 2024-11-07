#pragma once

#include "Service.h"

namespace EngineD
{
	class RigidBodyComponent;

	class PhysicsService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics);

		void Update(float deltaTime) override;
		void DebugUI() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		void Register(RigidBodyComponent* rbc);
		void Unregister(RigidBodyComponent* rbc);

		void SetEnable(bool enabled);

	private: 
		bool mEnabled = true;
	};
}