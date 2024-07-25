#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Renderer.h>

struct SDL_Window;

namespace CGL::Core
{
	CGL_DECLARE_LOG_CATEGORY(CoreApp);

	class Application
	{
	public:
	public:
		Application();
		~Application() = default;

		void Run();

	protected:
		virtual bool OnInit();
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnShutdown();
		virtual void OnResize(u32 width, u32 height);

		inline Graphics::Renderer* const GetRenderer() const { return m_renderer.get(); }

	protected:
		bool m_isRunning;
		SDL_Window* m_window;
		std::unique_ptr<Graphics::Renderer> m_renderer;
	};
}