#include "OPENGLRendererImpl.h"

std::pair<int, int> GetWindowSize(SDL_Window* window) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    return std::make_pair(width, height);
}

namespace CGL::Graphics
{
    CGL_DEFINE_LOG_CATEGORY(OPENGLRendererImpl);

    OPENGLRendererImpl::OPENGLRendererImpl(SDL_Window* window)
        : gWindow(window)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        gContext = SDL_GL_CreateContext(gWindow);

        glewExperimental = GL_TRUE;
        m_glewError = glewInit();
        
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        SDL_GL_SetSwapInterval(1);        

        glViewport(0, 0, GetWindowSize(gWindow).first,GetWindowSize(gWindow).second);    
    }

    OPENGLRendererImpl::~OPENGLRendererImpl()
    {
        SDL_GL_DeleteContext(gContext);
    }

    void OPENGLRendererImpl::SetPrimitive(GLenum primitive)
    {
        m_primitive = primitive;
    }

    void OPENGLRendererImpl::SwapWindow()
    {
        SDL_GL_SwapWindow(gWindow);
    }
}  // namespace CGL::Graphics