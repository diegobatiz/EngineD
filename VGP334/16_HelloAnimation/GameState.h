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
	EngineD::Graphics::StandardEffect mStandardEffect;

	EngineD::Graphics::RenderObject mBall;
	EngineD::Graphics::RenderObject mGround;

	EngineD::Graphics::Animation mAnimation;
	float mAnimationTime = 0.0f;
};