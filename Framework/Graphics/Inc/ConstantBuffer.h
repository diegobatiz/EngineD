#pragma once

namespace EngineD::Graphics
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		virtual ~ConstantBuffer();

		void Initialize(uint32_t bufferSize);
		void Terminate();
		void Update(const void* data) const;

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;
		void BindDS(uint32_t slot) const;

		void SetContext(ID3D11DeviceContext* context);
		void SetDevice(ID3D11Device* device);

	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
		ID3D11DeviceContext* mContext = nullptr; 
		ID3D11Device* mDevice = nullptr;
	};

	template<class DataType>
	class TypedConstantBuffer final : public ConstantBuffer
	{
	public:
		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "Data type must be 16 byte aligned");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Update(const DataType& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};
}