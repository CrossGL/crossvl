#include "OPENGLRendererImpl.h"

namespace CGL::Graphics
{
    CGL_DEFINE_LOG_CATEGORY(OPENGLRendererImpl);

    OPENGLRendererImpl::OPENGLRendererImpl(SDL_Window* window)
        : m_gWindow(window)
        , m_primitive(GL_ZERO)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_gContext = SDL_GL_CreateContext(m_gWindow);

        glewExperimental = GL_TRUE;
        m_glewError = glewInit();
        
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        SDL_GL_SetSwapInterval(1);        

        SDL_GetWindowSize(window, &m_width, &m_height);
        glViewport(0, 0, m_width, m_height);
        glEnable(GL_DEPTH_TEST);
    }

    OPENGLRendererImpl::~OPENGLRendererImpl()
    {
        SDL_GL_DeleteContext(m_gContext);
    }

    void OPENGLRendererImpl::SetPrimitive(GLenum primitive)
    {
        m_primitive = primitive;
    }

    void OPENGLRendererImpl::SwapWindow()
    {
        SDL_GL_SwapWindow(m_gWindow);
    }
}  // namespace CGL::Graphics