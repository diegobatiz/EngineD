#pragma once

#include "MeshBuffer_D3D11.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Material.h"
#include "Model.h"

namespace EngineD::Graphics
{
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
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model);
	void CleanupRenderGroup(RenderGroup& renderGroup);

	template<class Effect>
	void DrawRenderGroup(Effect effect, RenderGroup renderGroup)
	{
		for (RenderObject& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}

}