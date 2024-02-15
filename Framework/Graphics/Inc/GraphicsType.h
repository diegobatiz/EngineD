#pragma once

#include "Colours.h"

namespace EngineD::Graphics
{
	class GraphicsType
	{
	public:
		virtual ~GraphicsType() {}

		virtual void Initialize(HWND window, bool fullscreen) = 0;
		virtual void Terminate() = 0;

		virtual void BeginRender() = 0;
		virtual void Render() = 0;
		virtual void EndRender() = 0;

		virtual void ToggleFullScreen() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void ResetRenderTarget() = 0;
		virtual void ResetViewport() = 0;

		virtual void SetClearColor(const Color& color) = 0;

		template<class VertexType>
		void CreateMeshBuffer(const std::vector<VertexType>& vertices) {}
		template<class MeshType>
		void CreateMeshBuffer(const MeshType& mesh) {}
		template<class VertexType>
		void CreateShaders(std::filesystem::path filePath) {};

		virtual uint32_t GetBackBufferWidth() const = 0;
		virtual uint32_t GetBackBufferHeight() const = 0;

		virtual float GetBackBufferAspectRatio() const = 0;
	};
}
