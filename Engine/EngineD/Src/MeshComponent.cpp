#include "Precompiled.h"
#include "MeshComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
	RenderObjectComponent::Deserialize(value);

	Model::MeshData& meshData = mModel.meshData.emplace_back();
	Model::MaterialData& matData = mModel.materialData.emplace_back();
	if (value.HasMember("Shape"))
	{
		const auto& shapeData = value["Shape"].GetObj();
		if (shapeData.HasMember("Type"))
		{
			std::string shapeType = shapeData["Type"].GetString();
			if (shapeType == "Sphere")
			{
				uint32_t slices = static_cast<uint32_t>(shapeData["Slices"].GetInt());
				uint32_t rings = static_cast<uint32_t>(shapeData["Rings"].GetInt());
				float radius = shapeData["Radius"].GetFloat();
				meshData.mesh = MeshBuilder::CreateSphere(slices, rings, radius);
			}
			else if (shapeType == "Plane")
			{
				uint32_t rows = static_cast<uint32_t>(shapeData["Rows"].GetInt());
				uint32_t columns = static_cast<uint32_t>(shapeData["Columns"].GetInt());
				float spacing = shapeData["Spacing"].GetFloat();
				meshData.mesh = MeshBuilder::CreateHorizontalPlane(rows, columns, spacing);
			}
			else if (shapeType == "Cube")
			{
				float size = shapeData["Size"].GetFloat();
				meshData.mesh = MeshBuilder::CreateCube(size);
			}
			else if (shapeType == "SpriteQuad")
			{
				float width = shapeData["Width"].GetFloat();
				float height = shapeData["Height"].GetFloat();
				meshData.mesh = MeshBuilder::CreateSpriteQuad(width, height);
			}
			else
			{
				ASSERT(false, "MeshComponent: invalid shape type %s", shapeType.c_str());
			}
		}
		else
		{
			ASSERT(false, "MeshComponent: must specify a shape type");
		}
	}
	else
	{
		ASSERT(false, "MeshComponent: must have shape data");
	}

	if (value.HasMember("Material"))
	{
		const auto& materialData = value["Material"].GetObj();
		if (materialData.HasMember("Ambient"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			matData.material.ambient.r = color[0].GetFloat();
			matData.material.ambient.g = color[1].GetFloat();
			matData.material.ambient.b = color[2].GetFloat();
			matData.material.ambient.a = color[3].GetFloat();
		}
		if (materialData.HasMember("Diffuse"))
		{
			const auto& color = materialData["Diffuse"].GetArray();
			matData.material.diffuse.r = color[0].GetFloat();
			matData.material.diffuse.g = color[1].GetFloat();
			matData.material.diffuse.b = color[2].GetFloat();
			matData.material.diffuse.a = color[3].GetFloat();
		}
		if (materialData.HasMember("Specular"))
		{
			const auto& color = materialData["Specular"].GetArray();
			matData.material.specular.r = color[0].GetFloat();
			matData.material.specular.g = color[1].GetFloat();
			matData.material.specular.b = color[2].GetFloat();
			matData.material.specular.a = color[3].GetFloat();
		}
		if (materialData.HasMember("SpecularPower"))
		{
			matData.material.power = materialData["SpecularPower"].GetFloat();
		}
	}
	if (value.HasMember("Textures"))
	{

	}
}

const Model& MeshComponent::GetModel() const
{
}
