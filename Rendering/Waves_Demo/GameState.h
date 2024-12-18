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

	EngineD::Graphics::RenderObject m_Water;
	EngineD::Graphics::RenderObject mSky;
	EngineD::Graphics::WaveEffect m_WaveEffect;
	EngineD::Graphics::AtmosphereEffect mAtmosphereEffect;

	EngineD::WaveLoaderComponent mWaveLoader;

	EngineD::Color mColor = EngineD::Colors::Black;
};