#pragma once

#include "CustomTypeIds.h"

class CustomPlayerControllerComponent;

class SnowRenderService : public EngineD::Service
{
public:
	SET_TYPE_ID(CustomServiceId::SnowRenderService);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
	void Deserialize(const rapidjson::Value& value) override;

	void RegisterPlayer(const EngineD::TransformComponent* player);

private:
	void Save();

	std::filesystem::path mLevelFilePath = "";

	const EngineD::CameraService* mCameraService = nullptr;
	const EngineD::Graphics::Transform* mPlayerTransform = nullptr;
	const CustomPlayerControllerComponent* mPlayerController;

	EngineD::Graphics::RenderObject mQuad;
	EngineD::Graphics::RenderObject mSnow;

	EngineD::Physics::SnowParticleSystemInfo mSnowParticleInfo;
	EngineD::Physics::SnowParticleSystem mSnowParticles;

	EngineD::Graphics::DirectionalLight mDirectionalLight;

	EngineD::Graphics::SnowEffect mSnowEffect;
	EngineD::Graphics::PlayerPositionMapEffect mPlayerPositionEffect;
	EngineD::Graphics::ParticleSystemEffect mParticleEffect;

	float mFPS = 0;
};