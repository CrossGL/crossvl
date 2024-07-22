#include "Win32Utils.h"

namespace CGL::Core::Platform::Win32::Utils
{
	void GetAdjustedWindowSize(DWORD style, DWORD exStyle, u32& width, u32& height)
	{
		RECT wr{ 0, 0, LONG(width), LONG(height) };
		::AdjustWindowRectEx(&wr, style, FALSE, exStyle);

		width = u32(wr.right - wr.left);
		height = u32(wr.bottom - wr.top);
	}

	void GetDesktopCentreForWindow(u32 width, u32 height, u32& x, u32& y)
	{
		RECT desktopRect{};
		::GetWindowRect(::GetDesktopWindow(), &desktopRect);
		x = (desktopRect.right / 2) - (width / 2);
		y = (desktopRect.bottom / 2) - (height / 2);
	}
}