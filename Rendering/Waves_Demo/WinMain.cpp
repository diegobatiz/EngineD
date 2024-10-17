#include <EngineD/Inc/EngineD.h>

#include "GameState.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	App& myApp = EngineD::MainApp();
	myApp.AddState<GameState>("GameState");

	AppConfig config;
	config.appName = L"Wave";
	config.winHeight = 540;
	config.winWidth = 960;

	myApp.Run(config);

	return (0);
}