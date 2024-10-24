#pragma once

#include "Component.h"

namespace EngineD
{
	class RenderObjectComponent : public Component
	{
	public: 
		void Initialize() override;
		void Terminate() override;

		void Deserialize(const rapidjson::Value& value) override;

		virtual const bool CanCastShadow() { return mCastShadow; } const
		virtual Graphics::ModelId GetModelId() const { return 0; }
		virtual const Graphics::Model& GetModel() const = 0;

	private:
		bool mCastShadow = true;
	};
}