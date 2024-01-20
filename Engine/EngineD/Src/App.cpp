#include "Precompiled.h"
#include "App.h"

using namespace EngineD;
using namespace EngineD::Core;

void App::Run(const AppConfig& config)
{
	LOG("App Started: %.3f", TimeUtil::GetTime());

	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);
	ASSERT(myWindow.IsActive(), "Failed to create a window");

	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		float deltaTime = TimeUtil::GetDeltaTime();
		if (!myWindow.IsActive())
		{
			Quit();
		}
	}

	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
