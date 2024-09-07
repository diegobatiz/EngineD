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
	void OnEvent2();
	void OnEvent3();

	void OnSpaceEvent(const EngineD::Event* event);
	void OnAnimEvent(const EngineD::Event* event);

	EngineD::Graphics::DirectionalLight mDirectionalLight;
	EngineD::Graphics::Camera mCamera;

	EngineD::Graphics::ModelId mModelId;
	EngineD::Graphics::RenderGroup mCharacter;
	EngineD::Graphics::Animator mCharacterAnimator;
	EngineD::Graphics::StandardEffect mStandardEffect;

	EngineD::ListenerId mSpaceEventId = 0;
	EngineD::ListenerId mAnimEventId = 0;

	std::vector<EngineD::Audio::SoundId> mEventSoundIds;
	EngineD::Graphics::Animation mEventAnimation;
	float mEventAnimationTime = 0.0f;

	int mAnimIndex = 1;
	bool mDrawSkeleton = false;
};