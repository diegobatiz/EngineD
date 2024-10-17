#pragma once

#include "Component.h"

namespace EngineD
{
	class TransformComponent final : public Component, public Graphics::Transform
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);

		void DebugUI() override;
	};
}