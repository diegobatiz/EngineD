#include <EngineD/Inc/EngineD.h>

#include "GameState.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	App& myApp = EngineD::MainApp();
	myApp.AddState<TransformState>("TransformState");
	myApp.AddState<SphereState>("SphereState");
	myApp.AddState<AABBState>("AABBState");
	myApp.AddState<FilledAABBState>("FilledAABBState");
	myApp.AddState<DrawingState>("DrawingState");

	AppConfig config;
	config.appName = L"Hello Simple Draw";

	myApp.Run(config);

	return (0);
}