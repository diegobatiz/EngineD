#pragma once

namespace EngineD::Graphics
{
	class HullShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "HSControlPoint");
		void Terminate();
		void Bind();

	protected:
		ID3D11HullShader* mHullShader;

		ID3D11Device* mDevice;
		ID3D11RasterizerState* mRasterizerState;
	};
}