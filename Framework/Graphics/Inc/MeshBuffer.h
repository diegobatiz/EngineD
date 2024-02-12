#pragma once

namespace EngineD::Graphics
{
	class MeshBuffer
	{
	public:
		enum class Topology
		{
			Points,
			Lines,
			Triangles
		};

		virtual void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount) {}
		virtual void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount) {};
		virtual void Terminate() = 0;
		virtual void SetTopology(Topology topology) = 0;

	protected:
		uint32_t mVertexSize;
		uint32_t mVertexCount;
		uint32_t mIndexCount;
	};

}