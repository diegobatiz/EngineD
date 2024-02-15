#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "GraphicsType_D3D11.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Core;

namespace
{
	std::unique_ptr<GraphicsType> sGraphicsSystem;
	WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK GraphicsSystem::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem != nullptr)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
			const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
			sGraphicsSystem->Resize(width, height);
			break;
		}
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "GraphicsSystem: is already initialized");

#ifdef D3D11
	sGraphicsSystem = std::make_unique<Graphics_D3D11>();
#endif

	ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem: has not been initialized");

	sGraphicsSystem->Initialize(window, fullscreen);

	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sWindowMessageHandler.Unhook();

		sGraphicsSystem->Terminate();
		sGraphicsSystem.reset();
	}
}

#ifdef D3D11
Graphics_D3D11* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem: is not initialized");
	Graphics_D3D11* system = dynamic_cast<Graphics_D3D11*>(sGraphicsSystem.get());
	return system;
}
#else
GraphicsType* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem: is not initialized");
	return sGraphicsSystem.get();
}
#endif