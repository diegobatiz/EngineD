#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Model.h"
#include "Colours.h"
#include "SimpleDraw.h"
#include "Animator.h"

using namespace EngineD;
using namespace EngineD::Graphics;

namespace
{
	using namespace EngineD::Graphics::AnimationUtil;

	void ComputeBoneTransformsRecursive(const Bone* bone, BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone != nullptr)
		{
			if (animator != nullptr)
			{
				boneTransforms[bone->index] = animator->GetToParentTransform(bone);
			}
			else
			{
				boneTransforms[bone->index] = bone->toParentTransform;
			}

			if (bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
			}

			for (const Bone* child : bone->children)
			{
				ComputeBoneTransformsRecursive(child, boneTransforms, animator);
			}
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelId id, BoneTransforms& boneTransforms, const Animator* animator)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);
		ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
	}
}

void AnimationUtil::ApplyBoneOffsets(ModelId id, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}

void AnimationUtil::DrawSkeleton(ModelId id, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Vector3 posA = Math::GetTranslation(boneTransforms[bone->index]);
				const Vector3 posB = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				if (Math::DistanceSqr(posA, posB) > 0.0001f)
				{
					SimpleDraw::AddLine(posA, posB, Colors::Blue);
					SimpleDraw::AddSphere(10, 10, 0.03f, posA, Colors::Pink);
				}
			}
		}
	}
}

