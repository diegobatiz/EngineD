#include "Precompiled.h"
#include "DynamicInstancedMeshBuffer.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void DynamicInstancedMeshBuffer::UpdateInstanceBuffer(const std::vector<SnowParticle>& mParticles)
{
}

void DynamicInstancedMeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, uint32_t maxInstanceCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
	CreateIndexBuffer(indices, indexCount);
	CreateInstanceBuffer(maxInstanceCount);
}

void DynamicInstancedMeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mInstanceBuffer);
}

void DynamicInstancedMeshBuffer::Render() const
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offsets[2];
	UINT strides[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = mVertexSize;
	strides[1] = sizeof(InstanceData);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = mVertexBuffer;
	bufferPointers[1] = mInstanceBuffer;

	//update the instance buffer positions and change instance count

	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	context->DrawIndexedInstanced(mIndexCount, mInstanceCount, 0, 0, 0);
}

void DynamicInstancedMeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mDevice = GraphicsSystem::Get()->GetDevice();
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(vertexCount) * vertexSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = mDevice->CreateBuffer(&vertexBufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex data");
}

void DynamicInstancedMeshBuffer::CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount)
{
	mIndexCount = indexCount;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(indexCount) * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = mDevice->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index data");
}

void DynamicInstancedMeshBuffer::CreateInstanceBuffer(uint32_t maxInstanceCount)
{
	D3D11_BUFFER_DESC instanceBufferDesc = {};
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceData) * maxInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = mDevice->CreateBuffer(&instanceBufferDesc, nullptr, &mInstanceBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create instance data");
}
