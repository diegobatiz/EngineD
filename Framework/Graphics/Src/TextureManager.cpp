#include "Precompiled.h"
#include "TextureManager.h"

using namespace EngineD;
using namespace EngineD::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sInstance;
}

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sInstance == nullptr, "TextureManager: already initialized");
	sInstance = std::make_unique<TextureManager>();
	sInstance->SetRootDirectory(root);
}

void TextureManager::StaticTerminate()
{
	sInstance.reset();
}

TextureManager* TextureManager::Get()
{
	ASSERT(sInstance != nullptr, "TextureManager: is not initialized");
	return sInstance.get();
}

TextureManager::~TextureManager()
{
	for (auto& [id, texture] : mInventory)
	{
		texture->Terminate();
	}
	mInventory.clear();
}

void TextureManager::SetRootDirectory(const std::filesystem::path& root)
{
	mRootDirectory = root;
}

TextureID TextureManager::LoadTexture(const std::filesystem::path& filename, bool useRootDir)
{
	const std::size_t textureId = std::filesystem::hash_value(filename);
	auto [iter, success] = mInventory.insert({ textureId, nullptr });
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize((useRootDir) ? mRootDirectory / filename : filename);
	}

	return textureId;
}

const Texture* TextureManager::GetTexture(TextureID id) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		return iter->second.get();
	}

	return nullptr;
}

void TextureManager::BindVS(TextureID id, uint32_t slot)
{
	if (id == 0)
	{
		return;
	}

	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

void TextureManager::BindPS(TextureID id, uint32_t slot)
{
	if (id == 0)
	{
		return;
	}

	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}
