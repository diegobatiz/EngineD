#pragma once

namespace EngineD::Graphics
{
	class VertexShader_D3D11 final
	{
	public:
		template<class VertexType>
		void Initialize(const std::filesystem::path& filePath)
		{
			Initialize(filePath, VertexType::Format);
		}
		void Initialize(const std::filesystem::path& filePath, uint32_t format);
		void Terminate();
		void Bind(ID3D11DeviceContext* context);

		void InitDevice(ID3D11Device* device)
		{
			mDevice = device;
		}

	protected:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;

		ID3D11Device* mDevice;
	};
}