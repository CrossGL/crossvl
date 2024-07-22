#pragma once
#include <Core/Common.h>
#include <Windows.h>

namespace CGL::Core::Platform::Win32::Utils
{
	void GetAdjustedWindowSize(DWORD style, DWORD exStyle, u32& width, u32& height);
	void GetDesktopCentreForWindow(u32 width, u32 height, u32& x, u32& y);
}
