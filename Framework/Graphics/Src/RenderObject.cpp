#include "Precompiled.h"
#include "RenderObject.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
	texture.Terminate();
}