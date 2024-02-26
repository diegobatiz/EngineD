#include <EngineD/Inc/EngineD.h>

#include "GameState.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	App& myApp = EngineD::MainApp();
	myApp.AddState<CubeState>("CubeState");
	myApp.AddState<RectState>("RectState");
	myApp.AddState<PlaneState>("PlaneState");
	myApp.AddState<SphereState>("SphereState");
	myApp.AddState<CylinderState>("CylinderState");
	myApp.AddState<SkyBoxState>("SkyBoxState");
	myApp.AddState<SkySphereState>("SkySphereState");

	AppConfig config;
	config.appName = L"Hello Cube";

	myApp.Run(config);

	return (0);
}