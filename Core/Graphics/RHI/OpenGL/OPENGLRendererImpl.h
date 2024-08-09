#pragma once
#include "Core/Logging/Log.h"
#include "SDL2/SDL_opengl.h"
#include <Core/Graphics/Types.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(OPENGLRendererImpl);

    class OPENGLRendererImpl
    {
    public:
        explicit OPENGLRendererImpl(SDL_Window* window);
        ~OPENGLRendererImpl();

        void SwapWindow();

    private:
        SDL_Window* gWindow;
        SDL_GLContext gContext;
        GLenum glewError;
    };
}  // namespace CGL::Graphics
