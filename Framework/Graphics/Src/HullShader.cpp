#include "Precompiled.h"
#include "HullShader.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void HullShader::Initialize(const std::filesystem::path& filePath, const char* entryPoint)
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
		entryPoint, "hs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile hull shader");

	hr = mDevice->CreateHullShader(
		shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(), 
		nullptr,
		&mHullShader);

	ASSERT(SUCCEEDED(hr), "Failed to create hull shader");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void HullShader::Terminate()
{
	SafeRelease(mHullShader);
}

void HullShader::Bind()
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->HSSetShader(mHullShader, nullptr, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}
