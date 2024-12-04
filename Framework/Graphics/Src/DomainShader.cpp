#include "Precompiled.h"
#include "DomainShader.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void DomainShader::Initialize(const std::filesystem::path& filePath, const char* entryPoint)
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
		entryPoint, "ds_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile domain shader");

	hr = mDevice->CreateDomainShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mDomainShader);

	ASSERT(SUCCEEDED(hr), "Failed to create domain shader");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void DomainShader::Terminate()
{
	SafeRelease(mDomainShader);
}

void DomainShader::Bind()
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->DSSetShader(mDomainShader, nullptr, 0);
}
