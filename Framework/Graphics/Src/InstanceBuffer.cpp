#include "Precompiled.h"
#include "InstanceBuffer.h"

#include "Noise.h"
#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void EngineD::Graphics::GrassBuffer::SetSideSize(uint32_t length)
{
	mSideLength = length;
}

void EngineD::Graphics::GrassBuffer::SetDensity(uint32_t density)
{
	mDensity = density;
}

void GrassBuffer::SetInstanceAmount(uint32_t amount)
{
	mInstanceCount = amount;
}

void GrassBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
	CreateIndexBuffer(indices, indexCount);
}

void GrassBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mInstanceBuffer);
}

void GrassBuffer::Render() const
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offsets[2];
	UINT strides[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = mVertexSize;
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = mVertexBuffer;
	bufferPointers[1] = mInstanceBuffer;

	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	context->DrawIndexedInstanced(mIndexCount, mInstanceCount, 0, 0, 0);
}

void GrassBuffer::SetTerrain(const SimpleTerrain& terrain)
{
	mTerrain = &terrain;
}

void GrassBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
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

	mInstanceCount = mSideLength * mSideLength * mDensity * mDensity;

	InstanceType* instances = new InstanceType[mInstanceCount];
	
	int index = mInstanceCount - 1;

	float frequency = 0.5f;
	float heightFrequency = 0.2f;
	float amplitude = 0.3f;
	uint32_t x = mSideLength * mDensity;
	uint32_t y = mSideLength * mDensity;
	float length = mSideLength / mDensity;
	float step = 1.0f / mDensity;

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			Math::Vector3 position = { ((j * step) - length), 0.0f, ((i * step) - length) };
			float noiseX = Noise::NoiseSimplex({ position.z * frequency, position.x * frequency });
			float noiseZ = Noise::NoiseSimplex({ -position.x * frequency, -position.z * frequency });
			float noiseY = Noise::NoiseSimplex({ noiseZ, noiseX });

			noiseX *= amplitude;
			noiseZ *= amplitude;

			position.x += noiseX;
			position.z += noiseZ;
			position.y += noiseY;

			Math::Vector3 terrainPosition = position;
			terrainPosition.x += length;
			terrainPosition.x -= noiseX;
			terrainPosition.y = 0.0f;
			terrainPosition.z += length;
			terrainPosition.z -= noiseZ;
			float height = mTerrain->GetVertexHeight(j, i);

			instances[index].id.x = position.x;
			instances[index].id.y = position.y;
			instances[index].id.z = position.z;
			instances[index].id.a = height;
			index--;
		}
	}
	

	D3D11_BUFFER_DESC instanceBufferDesc = {};
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * mInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA instanceData = {};
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	hr = mDevice->CreateBuffer(&instanceBufferDesc, &instanceData, &mInstanceBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create instance data");

	delete[] instances;
	instances = 0;
}

void GrassBuffer::CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount)
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

void GrassBuffer::CreateComputeShader()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(mInstanceCount) * sizeof(InstanceType);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	InstanceType* instances = new InstanceType[mInstanceCount];
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = instances;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = mDevice->CreateBuffer(&bufferDesc, &initData, &mComputeBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index data");
}