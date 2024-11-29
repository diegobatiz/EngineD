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
	EngineD::Graphics::DirectionalLight m_DirectionalLight;
	EngineD::Graphics::Camera m_Camera;

	EngineD::Graphics::SnowEffect mSnowEffect;
	EngineD::Graphics::PlayerPositionMapEffect mPositionMapEffect;

	EngineD::Graphics::RenderObject mSnow;
	EngineD::Graphics::RenderObject mQuad;
	EngineD::Graphics::Transform mTransform;
};