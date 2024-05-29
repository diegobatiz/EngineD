#include "Precompiled.h"
#include "ShadowEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void ShadowEffect::Initialize()
{
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();

	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(2000.0f);

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ShadowEffect::Begin()
{
	UpdateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);

	mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mLightCamera.GetViewMatrix();
	const Math::Matrix4 matProj = mLightCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}

void ShadowEffect::DebugUI()
{
}

void ShadowEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
}

void ShadowEffect::SetFocus(const Math::Vector3& focusPosition)
{
}

void ShadowEffect::SetSize(float size)
{
}

const Camera& ShadowEffect::GetLightCamera() const
{
	// // O: ient here
}

const Texture& ShadowEffect::GetDepthMap() const
{
	// // O: insert return statement here
}

void ShadowEffect::UpdateLightCamera()
{
}
