#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Renderer.h>
#include <SDL2/SDL_events.h>

struct SDL_Window;

namespace CGL::Core
{
	CGL_DECLARE_LOG_CATEGORY(CoreApp);

	class Application
	{
	public:
	public:
		Application(std::string_view name, i32 argc, char** argv);
		~Application() = default;

		inline SDL_Window* GetWindow() const noexcept { return m_window; }

		void Run();

	protected:
		virtual bool OnInit();
		virtual void OnUpdate(const SDL_Event& e) = 0;
		virtual void OnRender() = 0;
		virtual void OnShutdown();
		virtual void OnResize(u32 width, u32 height);

		inline Graphics::Renderer* GetRenderer() const { return m_renderer.get(); }

	protected:
		bool m_isRunning;

	private:
		std::string m_name;
		SDL_Window* m_window;
		std::unique_ptr<Graphics::Renderer> m_renderer;
	};
}