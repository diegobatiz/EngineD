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
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
	}
}

void CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
