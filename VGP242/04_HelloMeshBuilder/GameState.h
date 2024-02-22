#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	EngineD::Graphics::Camera mCamera;
	EngineD::Graphics::MeshPX mMesh;

	EngineD::Graphics::Texture mTexture;
	EngineD::Graphics::Sampler mSampler;
};