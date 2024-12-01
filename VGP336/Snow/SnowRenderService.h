#pragma once

#include "CustomTypeIds.h"

class SnowRenderService : public EngineD::Service
{
public:
	SET_TYPE_ID(CustomServiceId::SnowRenderService);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void RegisterPlayer(const TransformComponent* player);
	void UnegisterPlayer(const TransformComponent* player);

	void Register(const RenderObjectComponent* roc);
	void Unregister(const RenderObjectComponent* roc);

private:
	const CameraService* mCameraService = nullptr;
	const TransformComponent* mPlayerTransform = nullptr;

	EngineD::Graphics::RenderObject mQuad;
	EngineD::Graphics::RenderObject mSnow;

	EngineD::Graphics::DirectionalLight mDirectionalLight;
	EngineD::Graphics::SnowEffect mSnowEffect;
	EngineD::Graphics::PlayerPositionMapEffect mPlayerPositionEffect;

	struct Entry
	{
		const RenderObjectComponent* renderComponent = nullptr;
		const TransformComponent* transformComponent = nullptr;
		Graphics::RenderGroup renderGroup;
	};

	using RenderEntries = std::vector<Entry>;
	RenderEntries mRenderEntries;

	float mFPS = 0;
};