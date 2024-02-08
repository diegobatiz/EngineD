#include "Precompiled.h"
#include "MeshBuffer.h"
#include "MeshTypes.h"
#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
	CreateIndexBuffer(indices, indexCount);
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
}

void MeshBuffer::Render()
{
	//auto context = GraphicsSystem
}

void MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
}

void MeshBuffer::CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount)
{
}
