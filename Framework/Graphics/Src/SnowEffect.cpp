#include "Precompiled.h"
#include "SnowEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void SnowEffect::Initialize()
{
	mTransformBuffer.Initialize();

	std::filesystem::path shaderFile = "../../Assets/Shaders/Snow.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SnowEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
}

void SnowEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);

	mPositionMap->BindVS(0);
}

void SnowEffect::End()
{
	if (mPositionMap != nullptr)
	{
		Texture::UnbindPS(0);
	}
}

void SnowEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Math::Transpose(matFinal);

	mTransformBuffer.Update(transformData);

	renderObject.meshBuffer.Render();
}

void SnowEffect::DebugUI()
{

}
