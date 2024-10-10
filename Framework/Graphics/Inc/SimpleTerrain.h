#pragma once

#include "MeshTypes.h"

namespace EngineD::Graphics
{
	class SimpleTerrain final
	{
	public:
		void Initialize(const std::filesystem::path& fileName, float heightScale, Color color);
		void SetHeightScale(float heightScale);
		float GetHeightScale() const;
		float GetHeight(const Math::Vector3& position) const;
		float GetVertexHeight(const Math::Vector3& position) const;
		const MeshPC& GetMesh() const;
		float GetWidth() const;
		float GetLength() const;
		void SetOffset(float offset); 
		void SetDensity(float density);

	private:
		MeshPC mMesh;
		uint32_t mRows = 0;
		uint32_t mColumns = 0;
		float mHeightScale = 1.0f;
		float mOffset;
		float mDensity;
	};
}