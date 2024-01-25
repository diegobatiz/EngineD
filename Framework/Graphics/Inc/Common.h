#pragma once

#include <Core/Inc/Core.h>
#include <Math/Inc/DMath.h>

#define DX11

//make a wrapper around this
#ifdef DX11
#include <d3d11_1.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#endif

template<class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}