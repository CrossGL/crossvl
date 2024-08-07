#include "OPENGLRendererImpl.h"

namespace CGL::Graphics {
CGL_DEFINE_LOG_CATEGORY(OPENGLRendererImpl);

OPENGLRendererImpl::OPENGLRendererImpl(SDL_Window *window) : gWindow(window) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  gContext = SDL_GL_CreateContext(gWindow);

  glewExperimental = GL_TRUE;

  glewError = glewInit();

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  SDL_GL_SetSwapInterval(1);
}

OPENGLRendererImpl::~OPENGLRendererImpl() { SDL_GL_DeleteContext(gContext); }

void OPENGLRendererImpl::SwapWindow() { SDL_GL_SwapWindow(gWindow); }
} // namespace CGL::Graphics