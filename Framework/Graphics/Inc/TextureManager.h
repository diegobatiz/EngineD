#pragma once

#include "Texture.h"

namespace EngineD::Graphics
{
	using TextureID = std::size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static TextureManager* Get();

		TextureManager() = default;
		~TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager(const TextureManager&&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&&) = delete;

		void SetRootDirectory(const std::filesystem::path& root);

		TextureID LoadTexture(const std::filesystem::path& filename, bool useRootDir = true);
		const Texture* GetTexture(TextureID id) const;
		
		void BindVS(TextureID id, uint32_t slot);
		void BindPS(TextureID id, uint32_t slot);
		void BindDS(TextureID id, uint32_t slot);

	private:
		using Inventory = std::unordered_map<TextureID, std::unique_ptr<Texture>>;
		Inventory mInventory;

		std::filesystem::path mRootDirectory;

	};
}