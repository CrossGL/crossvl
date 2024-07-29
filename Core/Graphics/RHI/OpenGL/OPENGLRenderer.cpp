#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/RHI/OpenGL/OPENGLRendererImpl.h>

namespace CGL::Graphics
{
    #ifdef CGL_RHI_OPENGL
    void Renderer::Constructor_OPENGL(SDL_Window* window)
    {
        this->m_impl = new OPENGLRendererImpl(window);

        CGL_LOG(Renderer, Info, "OpenGL Renderer Initialized");
    }

    void Renderer::Destructor_OPENGL()
    {
        delete static_cast<OPENGLRendererImpl*>(m_impl);
        m_impl = nullptr;

        CGL_LOG(Renderer, Info, "OPENGL Renderer Destroyed");
    }

    OPENGLRendererImpl* Renderer::GetImpl() const
    {
        assert(GetAPI() == RHIType::OpenGL);
        return static_cast<OPENGLRendererImpl*>(m_impl);
    }

    void Renderer::BeginFrame_OPENGL()
    {
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    }
    
    void Renderer::OnRender_OPENGL()
    {
        

    }
    
    void Renderer::EndFrame_OPENGL()
    { 
        GetImpl()->SwapWindow();
    }

    void Renderer::Resize_OPENGL(u32 width, u32 height)
	{
	    glViewport(0, 0, width, height);
	}

    #endif // CGL_RHI_OPENGL
}