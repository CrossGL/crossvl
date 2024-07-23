#include "Application.h"

namespace CGL::Core
{
	CGL_DEFINE_LOG_CATEGORY(CoreApp);

	Application::Application()
		: Super("CrossGL Visualizer", 1280, 720)
		, m_isRunning(true)
	{
		CGL_LOG(CoreApp, Trace, "Core aplication created");
	}

	void Application::Run()
	{
		CGL_LOG(CoreApp, Trace, "Running application");

		OnInit();

		while (m_isRunning && Super::IsOpen())
		{
			Super::ProcessMessages();

			OnUpdate();
			OnRender();
		}

		OnShutdown();
	}

	bool Application::OnInit()
	{
		Super::Show();

		CGL_LOG(CoreApp, Debug, "Core application initialized");
		return true;
	}

	void Application::OnUpdate()
	{
		Super::Update();
	}

	void Application::OnRender()
	{
	}

	void Application::OnShutdown()
	{
		CGL_LOG(CoreApp, Debug, "Core application shutdown");
		Super::Close();
	}

	void Application::OnResize([[maybe_unused]] u32 width, [[maybe_unused]] u32 height)
	{
		CGL_LOG(CoreApp, Debug, "Core application resized {}x{}", width, height);
	}

#ifdef CGL_PLATFORM_WINDOWS
	LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_ERASEBKGND:
		{
			// Notify the OS that erasing will be handled by the application to prevent flicker
			return 1;
		}
		}

		return Super::HandleMessage(uMsg, wParam, lParam);
	}
#endif	
}
