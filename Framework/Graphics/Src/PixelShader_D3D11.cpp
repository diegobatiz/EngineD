#include "Precompiled.h"
#include "PixelShader_D3D11.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void PixelShader_D3D11::Initialize(const std::filesystem::path& filePath)
{
	if (mDevice == nullptr)
	{
		mDevice = GraphicsSystem::Get()->GetDevice();
	}

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = mDevice->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void PixelShader_D3D11::Terminate()
{
	SafeRelease(mPixelShader);
}

void PixelShader_D3D11::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShader(mPixelShader, nullptr, 0);
}
