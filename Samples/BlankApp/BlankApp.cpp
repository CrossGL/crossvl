#include "BlankApp.h"

namespace CGL
{
    CGL_DEFINE_LOG_CATEGORY(BlankApp);

    BlankApp::BlankApp(i32 argc, char** argv)
        : Super("[CGL] Blank App", argc, argv)
    {
        CGL_LOG(BlankApp, Trace, "Created Blank App");
    }

    BlankApp::~BlankApp()
    {
        CGL_LOG(BlankApp, Trace, "Destroyed Blank App");
    }

    bool BlankApp::OnInit()
    {
        if (!Super::OnInit())
            return false;

        CGL_LOG(BlankApp, Info, "Initialized Blank App");

        return true;
    }

    void BlankApp::OnUpdate([[maybe_unused]] const SDL_Event& e)
    {
        // Update app here
    }

    void BlankApp::OnRender()
    {
    }

    void BlankApp::OnResize(u32 width, u32 height)
    {
        Super::OnResize(width, height);
    }

    void BlankApp::OnShutdown()
    {
        CGL_LOG(BlankApp, Info, "Shutting down Blank App");
        Super::OnShutdown();
    }
}  // namespace CGL
