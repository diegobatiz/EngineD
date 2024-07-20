#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace EngineD::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float timte) const;
		float GetDuration() const;

	private:
		const Math::Vector3& GetPosition(float time) const;
		const Math::Quaternion& GetRotation(float time) const;
		const Math::Vector3& GetScale(float time) const;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}