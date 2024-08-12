#include "Application.h"
#include <SDL2/SDL.h>
#include <chrono>

namespace CGL::Core
{
    CGL_DEFINE_LOG_CATEGORY(CoreApp);

    bool g_isTestMode{ false };

    Application::Application(std::string_view name, i32 argc, char** argv)
        : m_isRunning(true)
        , m_name(name)
        , m_window(nullptr)
    {
        // Parse command line arguments
        for (int i = 1; i < argc; ++i)
        {
            if (std::string(argv[i]) == "-test")
            {
                CGL_LOG(CoreApp, Info, "Running Visualizer in test mode");
                m_name.append(" - TEST");
                g_isTestMode = true;
            }
        }

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::string err(SDL_GetError());
            CGL_LOG(CoreApp, Error, "Failed to initialize SDL: {}", err);
        }
        else
        {
            CGL_LOG(CoreApp, Trace, "Core aplication created");
        }
    }

    void Application::Run()
    {
        CGL_LOG(CoreApp, Trace, "Running application");

        auto startTime = std::chrono::steady_clock::now();
        OnInit();

        SDL_Event e;
        while (m_isRunning)
        {
            // Handle events on queue
            while (SDL_PollEvent(&e) != 0)
            {
                // User requests quit
                if (e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_isRunning = false;
                }
                else if (e.type == SDL_WINDOWEVENT)
                {
                    switch (e.window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                        OnResize(e.window.data1, e.window.data2);
                        break;
                    }
                }
            }

            OnUpdate(e);

            // Call begin and end frame before calling render itself
            m_renderer->BeginFrame();
            OnRender();
            m_renderer->EndFrame();

            // Check for test mode and elapsed time
            if (g_isTestMode)
            {
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                if (elapsedTime >= 5)  // Run for 5 Seconds
                {
                    CGL_LOG(CoreApp, Info, "Test run completed after 5 seconds. Shutting Down...");
                    m_isRunning = false;
                }
            }
        }
        OnShutdown();
    }

	bool Application::OnInit()
    {
		// Create SDL window
        u32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
#if defined(CGL_RHI_OPENGL)
        flags |= SDL_WINDOW_OPENGL;
#elif defined(CGL_RHI_VULKAN)
        flags |= SDL_WINDOW_VULKAN;
#elif defined(CGL_RHI_METAL)
        flags |= SDL_WINDOW_METAL;
#endif

        m_window = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, flags);
        if (!m_window)
        {
            std::string err(SDL_GetError());
            CGL_LOG(CoreApp, Error, "Failed to create SDL window: {}", err);
            return false;
        }

        // Create renderer
        m_renderer = std::make_unique<Graphics::Renderer>(m_window);
        if (!m_renderer)
        {
            CGL_LOG(CoreApp, Error, "Failed to create renderer");
            return false;
        }

        CGL_LOG(CoreApp, Debug, "Core application initialized");
        return true;
    }

    void Application::OnShutdown()
    {
        m_renderer.reset();

        SDL_DestroyWindow(m_window);
        SDL_Quit();

        CGL_LOG(CoreApp, Debug, "Core application shutdown");
    }

    void Application::OnResize(u32 width, u32 height)
    {
        if (m_renderer)
        {
            m_renderer->Resize(width, height);
        }
    }
}  // namespace CGL::Core
