#pragma once


#include "Component.h"

namespace EngineD
{
	struct OceanData;

	class WaveLoaderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Invalid);

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		Graphics::WaveEffect::OceanData GetData();
		void SetEffect(EngineD::Graphics::WaveEffect* waveEffect);

	private:
		std::filesystem::path mTemplateFilePath;
		Graphics::WaveEffect::OceanData mData;
		Graphics::WaveEffect* mEffect;
	};
}