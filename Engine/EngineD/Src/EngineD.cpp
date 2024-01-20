#include "Precompiled.h"
#include "EngineD.h"

EngineD::App& EngineD::MainApp()
{
	static App sApp;
	return sApp;
}