#pragma once

namespace EngineD::Graphics
{
	struct Model;

	namespace ModelIO
	{
		bool SaveModel(std::filesystem::path filePath, const Model& model);
		bool LoadModel(std::filesystem::path filePath, Model& model);
	}
}