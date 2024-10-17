#pragma once

namespace EngineD::Graphics
{
	class StructuredBuffer
	{
	public:
		StructuredBuffer() = default;
		virtual ~StructuredBuffer();

		void Initialize(uint32_t elementSize, uint32_t elementCount);
		void Terminate();
		void Update(const void* data) const;

		template<class DataType>
		void Initialize(const std::vector<DataType>& data)
		{
			Initialize(sizeof(DataType), data.size());
		}

		void BindVS(uint32_t slot);
		void BindPS(uint32_t slot);

	private:
		ID3D11Buffer* mStructuredBuffer = nullptr;
		ID3D11ShaderResourceView* mSRV = nullptr;
	};

	template<class DataType>
	class TypedStructuredBuffer final : public StructuredBuffer
	{
	public:
		void Initialize(const std::vector<DataType>& data)
		{
			static_assert(sizeof(DataType) % 16 == 0, "Data type must be 16 byte aligned");
			StructuredBuffer::Initialize(data);
		}
	};
}
