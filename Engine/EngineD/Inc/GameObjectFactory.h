#pragma once

namespace EngineD
{
	class GameObject;
	namespace GameObjectFactory
	{
		void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
	}
}