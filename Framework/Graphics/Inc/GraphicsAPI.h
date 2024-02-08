#pragma once

#include "Colours.h"
#include "VertexTypes.h"

class VertexType;

namespace EngineD::Graphics
{
	enum class Topology
	{
		Points,
		Lines,
		Triangles
	};

	class GraphicsAPI
	{
	public:
		virtual ~GraphicsAPI() {}

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
		virtual void SetTopology(Topology topology) = 0;

		virtual void CreateTriangles(const std::vector<VertexType>& vertices) = 0;
		virtual void CreateShaders(std::filesystem::path filePath) = 0;
	};
}
