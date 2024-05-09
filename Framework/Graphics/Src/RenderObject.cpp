#include "Precompiled.h"
#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup Graphics::CreateRenderGroup(const Model& model)
{
	auto TryLoadTexture = [](const auto& textureName)->TextureID
		{
			if (textureName.empty())
			{
				return 0;
			}

			return TextureManager::Get()->LoadTexture(textureName, false);
		};

	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if (meshData.materialIndex < model.materialData.size())
		{
			const Model::MaterialData& materialData = model.materialData[meshData.materialIndex];
			renderObject.material = materialData.material;
			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
			renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
			renderObject.specMapId = TryLoadTexture(materialData.specularMapName);
		}
	}

	return renderGroup;
}

void Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}

void Graphics::SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& position)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.transform.position = position;
	}
}
