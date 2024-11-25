#pragma once

#include "Component.h"

namespace EngineD
{
	class CameraComponent;

	class LookCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCamera);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime);

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

	private:
		CameraComponent* mCameraComponent = nullptr;
		float mTurnSpeed = 0.1f;
		bool mCanLook = true;
	};
}