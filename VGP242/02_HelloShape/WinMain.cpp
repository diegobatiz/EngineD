#include <EngineD/Inc/EngineD.h>

#include "GameState.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	App& myApp = EngineD::MainApp();
    myApp.AddState<HeartState>("HeartState");
	//myApp.AddState<DiamondState>("DiamondState");
	//myApp.AddState<TriforceState>("TriforceState");
	//myApp.AddState<TriangleState>("TriangleState");

	AppConfig config;
	config.appName = L"Hello Shape";

	myApp.Run(config);

	return (0);
}