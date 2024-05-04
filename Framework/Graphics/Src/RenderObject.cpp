#include "Precompiled.h"
#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup CreateRenderGroup(const Model& model)
{
	auto TryLoadTexture = [](const auto& textureName)->TextureID
		{
			if (textureName.empty())
			{
				return 0;
			}

			return TextureManager::Get()->LoadTexture();
		};

	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if (meshData.materialIndex < model.materialData.size())
		{
			//const Model::MaterialData& materialData = model.materialData.
		}
	}
}

void CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
