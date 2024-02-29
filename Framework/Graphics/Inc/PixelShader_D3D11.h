#pragma once

namespace EngineD::Graphics
{
	class PixelShader_D3D11 final
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind(ID3D11DeviceContext* context);
		void Bind();

	protected:
		ID3D11PixelShader* mPixelShader;

		ID3D11Device* mDevice;
	};
}