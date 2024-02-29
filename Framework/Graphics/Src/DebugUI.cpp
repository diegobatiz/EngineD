#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Core;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONUP:
		}

	}

	LRESULT CALLBACK DebugUIMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
	}
}

void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{	 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	if (docking)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (multiViewport)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	// hook up window listener
	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);
}	 
	 
void DebugUI::StaticTerminate()
{	 
	sWindowMessageHandler.Unhook();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}	 
	 
void DebugUI::SetTheme(Theme theme)
{	 
	switch (theme)
	{
	case Theme::Classic:
	{
		ImGui::StyleColorsClassic();
		break;
	}
	case Theme::Dark:
	{
		ImGui::StyleColorsDark();
		break;
	}
	case Theme::Light:
	{
		ImGui::StyleColorsLight();
		break;
	}
	default: break;
	}
}	 
	 
void DebugUI::BeginRender()
{	 
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}	 
	 
void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData();
}
