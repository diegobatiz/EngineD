#pragma once

#include <Core/Inc/Core.h>
#include <Math/Inc/DMath.h>
#include <Physics/Inc/SnowParticleSystem.h>

#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <ImGui/Inc/imgui.h>

#include <DirectXTK/Inc/CommonStates.h>
#include <DirectXTK/Inc/SpriteBatch.h>
#include <FW1FontWrapper/Inc/FW1FontWrapper.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//#include <gl/GL.h>
//#include "GL/glext.h"
//#include "GL/wglext.h"

template<class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}