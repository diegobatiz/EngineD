#include <EngineD/Inc/EngineD.h>

using namespace EngineD;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App myApp;

	AppConfig config;
	config.appName = L"Hello Window";

	myApp.Run(config);

	return (0);
}