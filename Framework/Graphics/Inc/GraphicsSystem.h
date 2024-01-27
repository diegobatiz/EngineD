#pragma once

#include "Colours.h"

namespace EngineD::Graphics
{
	class GraphicsSystem
	{
	public:
		GraphicsSystem() = default;
		virtual ~GraphicsSystem() {}

		virtual void Initialize(HWND window, bool fullscreen) = 0;
		virtual void Terminate() = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		virtual void ToggleFullScreen() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void ResetRenderTarget() = 0;
		virtual void ResetViewport() = 0;

		virtual void SetClearColour(const Colour& colour) = 0;
		virtual void SetVSync(bool vSync) = 0;

		virtual uint32_t GetBackBufferWidth() const = 0;
		virtual uint32_t GetBackBufferHeight() const = 0;

		virtual float GetBackBufferAspectRatio() const = 0;
	};
}