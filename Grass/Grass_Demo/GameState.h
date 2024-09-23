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

	EngineD::Graphics::StandardEffect mStandardEffect;
	EngineD::Graphics::DirectionalLight mDirectionalLight;

	EngineD::Graphics::Camera mCamera;

	EngineD::Graphics::ModelId mModelIdA;
	EngineD::Graphics::RenderGroup mCharacterA;
	EngineD::Graphics::Animator mCharacterAnimatorA;

	EngineD::Graphics::ModelId mModelIdB;
	EngineD::Graphics::RenderGroup mCharacterB;
	EngineD::Graphics::Animator mCharacterAnimatorB;

	EngineD::Graphics::ModelId mModelIdC;
	EngineD::Graphics::RenderGroup mCharacterC;
	EngineD::Graphics::Animator mCharacterAnimatorC;

	
	float mAnimationTime = 0.0f;
	float mRotationTime = 0.0f;

	EngineD::Graphics::Animation mCameraAnimation;
	EngineD::Graphics::Animation mAnimationA;
	EngineD::Graphics::Animation mAnimationB;
	EngineD::Graphics::Animation mAnimationC;
	EngineD::Graphics::Animation mEarthAnimation;
	EngineD::Graphics::Animation mEvents;

	bool mPauseAnimation = false;
	bool mSpaceView = false;
	bool mRenderParticles = false;

	EngineD::Graphics::RenderObject mSky; 
	EngineD::Graphics::RenderObject mGround;
	EngineD::Graphics::RenderObject mEarth;


	EngineD::Graphics::ParticleSystemEffect mParticleEffect;
	EngineD::Physics::ParticleSystem mParticleSystem;


	void ChangeAnimation(int animId, EngineD::Graphics::Animator& animator);

	//Character A Events
	void IdleAnimationA();
	void WavingAnimationA();
	void ArgueAnimationA();
	void RunAnimationA();
	void CryAnimationA();
	void PointAnimationA();

	//Character B Events
	void ArgueAnimationB();
	void DeathAnimationB();
	void DeathLastFrameB();

	//Character C Events
	void AttackAnimationC();
	void DanceAnimationC();
	void IdleAnimationC();

	//Camera Events
	void SetCameraLookAtA();
	void SetCameraLookAtB();
	void SetCameraLookAtC();
	void SetCameraLookAtMiddle();
	void SetCameraLookAtDeath();
	void SetCameraLookAtDistance();
	void SetCameraLookAtEarth();

	//Lighting Events
	void FaceLightingLeft();
	void FaceLightingRight();
	void FaceLightingMiddle();
	void FaceLightingBack();

	//Other
	void ActivateParticleSystem()
	{
		mRenderParticles = true;
	}
};