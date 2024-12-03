#pragma once

#include "RenderObjectComponent.h"

namespace EngineD
{
	class MeshComponent final : public RenderObjectComponent
	{
	public: 
		SET_TYPE_ID(ComponentId::Mesh);

		void Deserialize(const rapidjson::Value& value) override;
		const Graphics::Model& GetModel() const override;

		const float GetSize() const;

	private:
		Graphics::Model mModel;
		float mSize = 0.0f;
	};
}