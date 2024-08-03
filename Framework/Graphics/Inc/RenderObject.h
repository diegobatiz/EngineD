#pragma once

#include "MeshBuffer_D3D11.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Material.h"
#include "ModelManager.h"
#include "Animator.h"

namespace EngineD::Graphics
{
	struct Model;

	struct RenderObject
	{
		void Terminate();

		Transform transform;
		MeshBuffer_D3D11 meshBuffer;

		Material material;

		TextureID diffuseMapId;
		TextureID normalMapId;
		TextureID specMapId;
		TextureID bumpMapId;

		ModelId modelId;
		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, ModelId id = 0, const Animator * animator = nullptr);
	[[nodiscard]] RenderGroup CreateRenderGroup(ModelId id, const Animator* animator = nullptr);
	void CleanupRenderGroup(RenderGroup& renderGroup);

	void SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& position);

	template<class Effect>
	void DrawRenderGroup(Effect& effect, RenderGroup& renderGroup)
	{
		for (RenderObject& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}

}