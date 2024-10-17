#include "Precompiled.h"
#include "StructuredBuffer.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

StructuredBuffer::~StructuredBuffer()
{
	ASSERT(mStructuredBuffer == nullptr, "ConstantBuffer: terminate must be called");
}

void StructuredBuffer::Initialize(uint32_t elementSize, uint32_t elementCount)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = elementSize * elementCount;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	HRESULT hr = device->CreateBuffer(&desc, nullptr, &mStructuredBuffer);
	ASSERT(SUCCEEDED(hr), "StructuredBuffer: failed to create buffer");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = elementCount;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;

	hr = device->CreateShaderResourceView(mStructuredBuffer, &srvDesc, &mSRV);
	ASSERT(SUCCEEDED(hr), "StructuredBuffer: failed to create shader resource view");
}

void StructuredBuffer::Terminate()
{
	SafeRelease(mStructuredBuffer);
	SafeRelease(mSRV);
}

void StructuredBuffer::Update(const void* data) const
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mStructuredBuffer, 0, nullptr, data, 0, 0);
}

void StructuredBuffer::BindVS(uint32_t slot)
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mSRV);
}

void StructuredBuffer::BindPS(uint32_t slot)
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mSRV);
}
