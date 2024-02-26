#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer_D3D11.h"
#include "PixelShader_D3D11.h"
#include "VertexShader_D3D11.h"
#include "VertexTypes.h"

#include "GraphicsSystem.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Math::Constants;

namespace
{
	class SimpleDrawImpl
	{
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
		void AddLine(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader_D3D11 mVertexShader;
		PixelShader_D3D11 mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer_D3D11 mMeshBuffer;

		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFaceVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};
	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
		std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx"; // change tp simpledraw.fx
		mVertexShader.Initialize<VertexPC>(shaderPath);
		mPixelShader.Initialize(shaderPath);
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.InitDevice(device);
		mMeshBuffer.Initialize();

		mLineVertexCount = 0;
		mFaceVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}
	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}
	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
	{
	}
	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
	{
	}
	void SimpleDrawImpl::Render(const Camera& camera)
	{
	}
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
}

void SimpleDraw::StaticTerminate()
{
}

void SimpleDraw::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
{
}

void SimpleDraw::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
{
}

void SimpleDraw::AddAABB(const Vector3& min, const Vector3& max, const Color& color)
{
}

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
}

void SimpleDraw::AddFilledAABB(const Vector3& min, const Vector3& max, const Color& color)
{
}

void SimpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
}

void SimpleDraw::AddSphere(uint32_t slice, uint32_t rings, float radius, const Color& color)
{
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
}

void SimpleDraw::AddGroundCircle(uint32_t slices, float radius, const Color& color)
{
}

void SimpleDraw::AddTransform(const Matrix4& matrix)
{
}

void SimpleDraw::Render(const Camera& camera)
{
}
