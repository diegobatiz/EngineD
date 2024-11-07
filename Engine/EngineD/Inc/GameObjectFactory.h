#pragma once

namespace EngineD
{
	class GameObject;

	//using CustomMake = std::function <Component*(const std::string&, GameObject&)>;
	//using CustomGet = std::function <Component*(const std::string&, GameObject&)>;

	namespace GameObjectFactory
	{
		void Make(const std::filesystem::path& templatePath, GameObject& gameObject);
		void OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject);
		//void SetCustomMake(CustomMake);
		//void SetCustomGet(CustomGet);
	}
}