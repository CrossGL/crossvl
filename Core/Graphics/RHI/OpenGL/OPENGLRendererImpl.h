#pragma once
#include <Core/Graphics/Types.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_opengl.h"

#include "Core/Logging/Log.h"

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(OPENGLRendererImpl);

    class OPENGLRendererImpl
    {
    public:
        OPENGLRendererImpl(SDL_Window* window);
        OPENGLRendererImpl() = delete;
        ~OPENGLRendererImpl();
        
        void SwapWindow();

    private:
        SDL_Window* gWindow;
        SDL_GLContext gContext;
        GLenum glewError;
    };
}