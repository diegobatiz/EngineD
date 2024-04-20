#pragma once

#include "MeshBuffer_D3D11.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Material.h"

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
	};
}