#pragma once

namespace EngineD::Graphics
{
	class InstanceVertexShader final
	{
	public:
		template<class VertexType>
		void Initialize(const std::filesystem::path& filePath)
		{
			Initialize(filePath, VertexType::Format); 
		}
		void Initialize(const std::filesystem::path& filePath, uint32_t format = 0);
		void Terminate();
		void Bind();

	protected:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;

		ID3D11Device* mDevice;
	};
}