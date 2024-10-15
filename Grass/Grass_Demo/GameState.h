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
	EngineD::Graphics::SimpleTerrain mTerrain;

	EngineD::Graphics::GrassBuffer mGrassBuffer;
	EngineD::Graphics::GrassEffect mGrassEffect; 
	EngineD::Graphics::FogEffect mTerrainEffect;
	EngineD::Graphics::RenderObject mGround;
};