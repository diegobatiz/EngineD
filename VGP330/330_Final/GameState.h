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

	EngineD::Graphics::Mesh circle1;
	EngineD::Graphics::Mesh circle2;
	EngineD::Graphics::Mesh circle3;
	EngineD::Graphics::Mesh horizontalPlane;
	EngineD::Graphics::Mesh verticalPlane;

	EngineD::Graphics::StandardEffect mStandardEffect;

	EngineD::Graphics::RenderObject circleObj1;
	EngineD::Graphics::RenderObject circleObj2;
	EngineD::Graphics::RenderObject circleObj3;
	EngineD::Graphics::RenderObject hPlane;
	EngineD::Graphics::RenderObject vPlane;

};