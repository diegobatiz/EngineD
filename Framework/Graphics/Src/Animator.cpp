#include "Precompiled.h"
#include "Animator.h"
#include "Animation.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void Animator::Initialize(ModelId id)
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
	if (clipIndex >= 0 && clipIndex < GetAnimationCount())
	{
		mClipIndex = clipIndex;
		mIsLooping = looping;
		mAnimationTick = 0.0f;
	}
}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mIsLooping)
	{
		while (mAnimationTick > animClip.tickDuration)
		{
			mAnimationTick -= animClip.tickDuration;
		}
	}
	else
	{
		mAnimationTick = std::min(mAnimationTick, animClip.tickDuration);
	}
}

bool Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

Math::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimations[bone->index].get();
	if (animation == nullptr)
	{
		return Math::Matrix4::Identity;
	}

	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();
}
