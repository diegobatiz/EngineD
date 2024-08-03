#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
public:
	virtual void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

protected:
	EngineD::Graphics::DirectionalLight mDirectionalLight;
	EngineD::Graphics::Camera mCamera;

	EngineD::Graphics::ModelId mModelId;
	EngineD::Graphics::ModelId mModelId2;
	EngineD::Graphics::RenderGroup mCharacter;
	EngineD::Graphics::RenderGroup mCharacter2;
	EngineD::Graphics::StandardEffect mStandardEffect;

	bool mDrawSkeleton = false;
};