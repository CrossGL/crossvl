#pragma once
#ifdef CGL_PLATFORM_WINDOWS
#include <Core/Platform/Win32/Window/Win32Window.h>
#elif CGL_PLATFORM_LINUX
#include <Core/Platform/Linux/Window/LinuxWindow.h>
#elif CGL_PLATFORM_MACOSX
#include <Core/Platform/MacOSX/Window/MacOSXWindow.h>
#else
#error "Window not supported on this platform"
#endif

namespace CGL::Core
{
#ifdef CGL_PLATFORM_WINDOWS
	using PlatformWindow = Platform::Win32::Window;
#elif CGL_PLATFORM_LINUX
	using PlatformWindow = Platform::Linux::Window;
#elif CGL_PLATFORM_MACOSX
	using PlatformWindow = Platform::MacOSX::Window;
#else
#error "No window implementation for this platform"
#endif

	template <typename IMPL = PlatformWindow>
	class Window
	{
	public:
		explicit Window(std::string_view title, u32 width, u32 height)
			: m_impl(title, width, height)
		{
		}

		IMPL& GetNative() { return m_impl; }
		const IMPL& GetNative() const { return m_impl; }

		void Show()
		{
			m_impl.Show();
		}

		void Close()
		{
			m_impl.Close();
		}

		bool IsOpen()
		{
			return m_impl.IsOpen();
		}

		void ProcessMessages()
		{
			m_impl.ProcessMessages();
		}

#ifdef CGL_PLATFORM_WINDOWS
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			return m_impl.HandleMessage(uMsg, wParam, lParam);
		}
#endif

	private:
		IMPL m_impl;
	};
}
