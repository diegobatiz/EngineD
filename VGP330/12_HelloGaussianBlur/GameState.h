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
	
	//Effects
	EngineD::Graphics::StandardEffect mStandardEffect;
	EngineD::Graphics::PostProcessingEffect mPostProcessEffect;
	EngineD::Graphics::GaussianBlurEffect mGaussianBlurEffect;

	//Objects
	EngineD::Graphics::RenderGroup mCharacter;
	EngineD::Graphics::RenderObject mGround;
	EngineD::Graphics::RenderObject mScreenQuad;

	//Render Targets
	EngineD::Graphics::RenderTarget mRenderTarget;
	EngineD::Graphics::RenderTarget mBlurRenderTarget;
};