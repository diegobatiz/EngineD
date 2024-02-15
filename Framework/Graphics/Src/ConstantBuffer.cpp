#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "ConstantBuffer: terminate must be called");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT hr = mDevice->CreateBuffer(&desc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "ConstantBuffer: failed to create buffer");
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Update(const void* data) const
{
	mContext->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	mContext->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	mContext->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::SetContext(ID3D11DeviceContext* context)
{
	mContext = context;
}

void ConstantBuffer::SetDevice(ID3D11Device* device)
{
	mDevice = device;
}
