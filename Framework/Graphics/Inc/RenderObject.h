#pragma once

#include "MeshBuffer_D3D11.h"
#include "TextureManager.h"
#include "Transform.h"

namespace EngineD::Graphics
{
	struct RenderObject
	{
	public:
		void Terminate();

		MeshBuffer_D3D11 meshBuffer;
		Transform transform;

		TextureID diffuseTextureId;
	};
}