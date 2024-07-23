#pragma once
#include <Core/Common.h>
#include <Core/Platform/Window.h>
#include <Core/Logging/Log.h>

namespace CGL::Core
{
	CGL_DECLARE_LOG_CATEGORY(CoreApp);

	class Application : public Window<PlatformWindow>
	{
	protected:
		using Super = Window<PlatformWindow>;

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
#ifdef CGL_PLATFORM_WINDOWS
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
#endif

	protected:
		bool m_isRunning;
	};
}