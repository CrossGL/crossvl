#pragma once
#include <Core/Common.h>
#include <Core/Logging/Log.h>

struct SDL_Window;

namespace CGL::Core
{
	CGL_DECLARE_LOG_CATEGORY(CoreApp);

	class Application
	{
	public:
		struct Config
		{
			u32 Width = 1280;
			u32 Height = 720;
			bool Fullscreen = false;
			bool IsVsync = true;
			
			// Choose RHI here
		};
	public:
		Application();
		~Application() = default;

		void Run();

	protected:
		virtual bool OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnShutdown();
		virtual void OnResize(u32 width, u32 height);

	protected:
		bool m_isRunning;
		SDL_Window* m_window;
	};
}