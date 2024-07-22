#include "Win32Window.h"

#pragma comment(lib, "Gdi32.lib")

namespace CGL::Core::Platform::Win32
{
	Window::Window(std::string_view title, u32 width, u32 height)
		: m_isOpen(false)
	{
		u32 windowWidth = width;
		u32 windowHeight = height;
		u32 posX = 0, posY = 0;

		Utils::GetAdjustedWindowSize(WS_OVERLAPPEDWINDOW, 0, windowWidth, windowHeight);
		Utils::GetDesktopCentreForWindow(windowWidth, windowHeight, posX, posY);

		Create(title, WS_OVERLAPPEDWINDOW, 0, posX, posY, windowWidth, windowHeight);
		m_isOpen = true;
	}

	Window::~Window()
	{
		::UnregisterClass(ClassName().data(), ::GetModuleHandle(NULL));
	}

	void Window::Show()
	{
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	}

	void Window::Close()
	{
		::DestroyWindow(m_hWnd);
	}

	void Window::ProcessMessages()
	{
		// Message pump
		MSG msg = { 0 };
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	std::string_view Window::ClassName() const { return "CGLWindow"; }

	LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			m_isOpen = false;
			return 0;
		}

		case WM_SIZE:
		{
			OnResize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}
