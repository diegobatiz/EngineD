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
	EngineD::Graphics::WaveEffect m_WaveEffect;

	EngineD::WaveLoaderComponent mWaveLoader;
};