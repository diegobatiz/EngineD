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

	//Objects
	EngineD::Graphics::RenderGroup mCharacter;
	EngineD::Graphics::RenderObject mGround;
	EngineD::Graphics::RenderObject mScreenQuad;

	EngineD::Graphics::Texture mCombineTexture;

	EngineD::Graphics::RenderTarget mRenderTarget;
};