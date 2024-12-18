#pragma once

#include "TypeIds.h"
#include "SaveUtil.h"

namespace EngineD
{
	class GameObject;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(const Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(const Component&&) = delete;

		virtual void Initialize() {}
		virtual void Terminate() {}
		virtual void Update(float deltaTime) {}
		virtual void DebugUI() {}

		virtual uint32_t GetTypeId() const = 0;

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {}
		virtual void Deserialize(const rapidjson::Value& value) {}

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}