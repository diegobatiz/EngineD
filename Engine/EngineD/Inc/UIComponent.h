#pragma once

#include "Component.h"

namespace EngineD
{
	class UIComponent : public Component
	{
	public: 
		virtual void Render() = 0;

	protected:
		virtual void ObtainWorldPositon(DirectX::XMFLOAT2& position) = 0;
	};
}