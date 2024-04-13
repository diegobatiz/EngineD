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
	void RenderObject(); 
	EngineD::Graphics::Camera mCamera;
	EngineD::Graphics::MeshPX mMesh;

	EngineD::Graphics::StandardEffect mStandardEffect;
	EngineD::Graphics::RenderObject mRenderObject

};