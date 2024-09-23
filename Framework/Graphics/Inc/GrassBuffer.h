#pragma once

namespace EngineD::Graphics
{
	class GrassBuffer 
	{
		struct InstanceType
		{
			uint32_t id;
		};

	public:
		GrassBuffer() = default;

		template<class MeshType>
		void Initialize(const MeshType& mesh)
		{
			Initialize(mesh.vertices.data(),
				static_cast<uint32_t>(sizeof(MeshType::VertexType)),
				static_cast<uint32_t>(mesh.vertices.size()),
				mesh.indices.data(),
				static_cast<uint32_t>(mesh.indices.size())
			);
		}

		void SetInstanceAmount(uint32_t amount);
		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);
		void Terminate();
		void Render() const;

	protected:
		void CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount);

		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
		ID3D11Buffer* mInstanceBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ID3D11Device* mDevice = nullptr;

		uint32_t mVertexSize{};
		uint32_t mVertexCount{};
		uint32_t mIndexCount{};
		uint32_t mInstanceCount{};
	};
}