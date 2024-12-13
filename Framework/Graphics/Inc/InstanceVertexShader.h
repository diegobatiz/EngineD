#pragma once

namespace EngineD::Graphics
{
	class InstanceVertexShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind();

	protected:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;

		ID3D11Device* mDevice;
	};
}