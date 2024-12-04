#pragma once

namespace EngineD::Graphics
{
	class DomainShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "DS");
		void Terminate();
		void Bind();

	protected:
		ID3D11DomainShader* mDomainShader;

		ID3D11Device* mDevice;
	};
}