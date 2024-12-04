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
	if (mDevice == nullptr)
	{
		mDevice = GraphicsSystem::Get()->GetDevice();
	}

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
	if (mContext == nullptr)
	{
		ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
		context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
		return;
	}
	mContext->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	if (mContext == nullptr)
	{
		ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
		context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
		return;
	}
	mContext->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	if (mContext == nullptr)
	{
		ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
		context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
		return;
	}
	mContext->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindDS(uint32_t slot) const
{
	if (mContext == nullptr)
	{
		ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
		context->DSSetConstantBuffers(slot, 1, &mConstantBuffer);
		return;
	}
	mContext->DSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::SetContext(ID3D11DeviceContext* context)
{
	mContext = context;
}

void ConstantBuffer::SetDevice(ID3D11Device* device)
{
	mDevice = device;
}
