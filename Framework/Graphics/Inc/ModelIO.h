#pragma once

namespace EngineD::Graphics
{
	struct Model;
	struct Animation;

	class AnimationIO
	{
	public:
		static void Write(FILE* file, const Animation& animation);
		static void Read(FILE* file, const Animation& animation);
	};

	namespace ModelIO
	{
		bool SaveModel(std::filesystem::path filePath, const Model& model);
		bool LoadModel(std::filesystem::path filePath, Model& model);		
		
		bool SaveMaterial(std::filesystem::path filePath, const Model& model);
		bool LoadMaterial(std::filesystem::path filePath, Model& model);

		bool SaveSkeleton(std::filesystem::path filePath, Model& model);
		bool LoadSkeleton(std::filesystem::path filePath, Model& model);

		bool SaveAnimation(std::filesystem::path filePath, const Model& model);
		bool LoadAnimation(std::filesystem::path filePath, const Model& model);
	}
}