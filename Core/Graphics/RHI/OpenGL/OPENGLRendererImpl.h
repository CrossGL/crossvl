#pragma once
#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include "SDL2/SDL_opengl.h"
#include "Core/Logging/Log.h"

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(OPENGLRendererImpl);

    class OPENGLRendererImpl
    {
    public:
        explicit OPENGLRendererImpl(SDL_Window* window);
        ~OPENGLRendererImpl();
      
        void SetPrimitive(GLenum primitive);
        inline GLenum GetPrimitive() { return m_primitive;}
        
        void SwapWindow();
    private:
        SDL_Window* m_gWindow;
        SDL_GLContext m_gContext;
        i32 m_width;
        i32 m_height;

        GLenum m_glewError;
        GLenum m_primitive;
    };
}  // namespace CGL::Graphics