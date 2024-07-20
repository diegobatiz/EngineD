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

	EngineD::Graphics::RenderGroup mCharacter;
	EngineD::Graphics::StandardEffect mStandardEffect;

	float mYaw = 0.0f;
	float mPitch = 0.0f;
	float mRoll = 0.0f;

	EngineD::Graphics::Transform mTransform;
};