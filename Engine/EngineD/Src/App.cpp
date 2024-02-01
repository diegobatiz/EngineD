#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace EngineD;
using namespace EngineD::Core;
using namespace EngineD::Graphics;

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

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

	Graphics_D3D11::StaticInitialize(myWindow.GetWindowHandle(), false);

	ASSERT(mCurrentState != nullptr, "App: need an app state");
	mCurrentState->Initialize();

	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		if (!myWindow.IsActive())
		{
			Quit();
			break;
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}
		float deltaTime = TimeUtil::GetDeltaTime();
		mCurrentState->Update(deltaTime);

		Graphics_D3D11* gs = Graphics_D3D11::Get();
		gs->BeginRender();
			mCurrentState->Render();
		gs->EndRender();
	}

	mCurrentState->Terminate();

	Graphics_D3D11::StaticTerminate();

	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
