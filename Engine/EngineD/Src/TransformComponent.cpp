#include "Precompiled.h"
#include "TransformComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void TransformComponent::DebugUI()
{
	ImGui::DragFloat3("Position", &position.x);
	ImGui::DragFloat3("Rotation", &rotation.x);
	ImGui::DragFloat3("Scale", &scale.x);

	SimpleDraw::AddTransform(GetMatrix4());
}
