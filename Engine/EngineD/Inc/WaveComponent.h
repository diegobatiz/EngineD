#pragma once


#include "Component.h"

namespace EngineD
{
	struct OceanData;

	class WaveLoaderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Wave);

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		Graphics::WaveEffect::OceanData GetData();
		Graphics::WaveEffect::LightData GetLightData();
		void SetEffect(EngineD::Graphics::WaveEffect* waveEffect);

	private:
		std::filesystem::path mTemplateFilePath;
		Graphics::WaveEffect::OceanData mData;
		Graphics::WaveEffect::LightData mLightData;
		Graphics::WaveEffect* mEffect;
	};
}