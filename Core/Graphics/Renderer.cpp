#include "Renderer.h"
#include "Core/Graphics/Types.h"
#include <SDL2/SDL_video.h>

namespace CGL::Graphics
{
    CGL_DEFINE_LOG_CATEGORY(Renderer);

    Renderer::Renderer(SDL_Window* window)
        : m_impl(nullptr), m_clearColor({ 1.0f, 0.0f, 1.0f, 1.0f })
    {
        i32 width, height;
        SDL_GetWindowSize(window, &width, &height);
        m_width  = u32(width);
        m_height = u32(height);

#if defined(CGL_RHI_DX11)
        CGL_LOG(Renderer, Debug, "Using RHI: DirectX11");
        Constructor_D3D11(window);
#elif defined(CGL_RHI_OPENGL)
        CGL_LOG(Renderer, Debug, "Using RHI: OpenGL");
        Constructor_OPENGL(window);
#elif defined(CGL_RHI_METAL)
        CGL_LOG(Renderer, Debug, "Using RHI: Metal");
        Constructor_METAL(window);
#endif
    }

    Renderer::~Renderer()
    {
#if defined(CGL_RHI_DX11)
        CGL_LOG(Renderer, Info, "Destructing RHI: DirectX11");
        Destructor_D3D11();
#elif defined(CGL_RHI_OPENGL)
        CGL_LOG(Renderer, Info, "Destructing RHI: OpenGL");
        Destructor_OPENGL();
#elif defined(CGL_RHI_METAL)
        CGL_LOG(Renderer, Info, "Destructing RHI: Metal");
        Destructor_METAL();
#endif
    }

    void Renderer::BeginFrame()
    {
#if defined(CGL_RHI_DX11)
        BeginFrame_D3D11();
#elif defined(CGL_RHI_OPENGL)
        BeginFrame_OPENGL();
#elif defined(CGL_RHI_METAL_)
        BeginFrame_METAL();
#endif
    }

    void Renderer::EndFrame()
    {
#if defined(CGL_RHI_DX11)
        EndFrame_D3D11();
#elif defined(CGL_RHI_OPENGL)
        EndFrame_OPENGL();
#elif defined(CGL_RHI_METAL)
        EndFrame_METAL();
#endif
    }

    void Renderer::Resize(u32 width, u32 height)
    {
        if (m_width == width && m_height == height)
        {
            return;
        }

        m_width  = width;
        m_height = height;

#if defined(CGL_RHI_DX11)
        Resize_D3D11(width, height);
#elif defined(CGL_RHI_OPENGL)
        Resize_OPENGL(width, height);
#elif defined(CGL_RHI_METAL)
        Resize_METAL(width, height);
#endif
    }

    void Renderer::SetClearColor(f32 r, f32 g, f32 b, f32 a)
    {
        m_clearColor = { r, g, b, a };
    }

    void Renderer::SetPrimitiveTopology(PrimitiveType topology)
    {
#if defined(CGL_RHI_DX11)
        SetPrimitiveTopology_D3D11(topology);
#elif defined(CGL_RHI_OPENGL)
        SetPrimitiveTopology_OPENGL(topology);
#elif defined(CGL_RHI_METAL)
        SetPrimitiveTopology_METAL(topology);
#endif
    }

    void Renderer::SetVertexShader(const VertexShader& shader)
    {
#if defined(CGL_RHI_DX11)
		SetVertexShader_D3D11(shader);
#elif defined(CGL_RHI_OPENGL)
		SetVertexShader_OPENGL(shader);
#endif
    }

    void Renderer::SetPixelShader(const PixelShader& shader)
    {
#if defined(CGL_RHI_DX11)
		SetPixelShader_D3D11(shader);
#elif defined(CGL_RHI_OPENGL)
		SetPixelShader_OPENGL(shader);
#endif
    }

    void Renderer::SetMaterial(const Material& material)
    {
        auto& vs = material.m_vs;
        auto& ps = material.m_ps;

        // Ensure all shaders are compiled before trying to set them
        assert(vs->State == ShaderState::Compiled);
        assert(ps->State == ShaderState::Compiled);

		    SetVertexShader(vs->Shader);
		    SetPixelShader(ps->Shader);

#if defined(CGL_RHI_OPENGL)
		    glUseProgram(material.GetMaterialID());
#endif
	  }

    void Renderer::SetVertexBuffer(const VertexBuffer& buffer)
    {
#if defined(CGL_RHI_DX11)
		    SetVertexBuffer_D3D11(buffer);
#elif defined(CGL_RHI_OPENGL)
		    SetVertexBuffer_OPENGL(buffer);
#endif
    }

    void Renderer::SetIndexBuffer(const IndexBuffer& buffer)
    {
#if defined(CGL_RHI_DX11)
		    SetIndexBuffer_D3D11(buffer);
#elif defined(CGL_RHI_OPENGL)
		    SetIndexBuffer_OPENGL(buffer);
#endif
    }

    bool Renderer::CompileVertexShader(const ShaderSource& source, VertexShader* outShader)
    {
        assert(outShader);

#if defined(CGL_RHI_DX11)
		    ShaderCompileResult result = CompileVertexShader_D3D11(source, outShader);
#elif defined(CGL_RHI_OPENGL)
		    ShaderCompileResult result = CompileVertexShader_OPENGL(source, outShader);
#endif
        ShaderCompiler::ReportResult(result, source.Name.data());
        return result.Status == ShaderCompileStatus::Success || result.Status == ShaderCompileStatus::HasWarnings;
    }

    bool Renderer::CompilePixelShader(const ShaderSource& source, PixelShader* outShader)
    {
        assert(outShader);

#if defined(CGL_RHI_DX11)
		    ShaderCompileResult result = CompilePixelShader_D3D11(source, outShader);
#elif defined(CGL_RHI_OPENGL)
		    ShaderCompileResult result = CompilePixelShader_OPENGL(source, outShader);
#endif
        ShaderCompiler::ReportResult(result, source.Name.data());
        return result.Status == ShaderCompileStatus::Success || result.Status == ShaderCompileStatus::HasWarnings;
    }

    VertexBuffer Renderer::CreateVertexBuffer(const BufferSource& source)
    {
#if defined(CGL_RHI_DX11)
		    return CreateVertexBuffer_D3D11(source);
#elif defined(CGL_RHI_OPENGL)
		    return CreateVertexBuffer_OPENGL(source);
#endif
    }

    IndexBuffer Renderer::CreateIndexBuffer(const BufferSource& source)
    {
#if defined(CGL_RHI_DX11)
		    return CreateIndexBuffer_D3D11(source);
#elif defined(CGL_RHI_OPENGL)
		    return CreateIndexBuffer_OPENGL(source);
#endif
    }

    bool Renderer::CompileMaterial(Material* material)
    {
        assert(material);
        bool result = true;

        // Compile vertex shader
        assert(material->m_vs->State == ShaderState::CompilePending);
        if (bool vsResult = CompileVertexShader(material->m_vs->Source, &material->m_vs->Shader))
        {
            result &= vsResult;
            material->m_vs->State = ShaderState::Compiled;
        }

        // Compile pixel shader
        assert(material->m_ps->State == ShaderState::CompilePending);
        if (bool psResult = CompilePixelShader(material->m_ps->Source, &material->m_ps->Shader))
        {
            result &= psResult;
            material->m_ps->State = ShaderState::Compiled;
        }

#if defined(CGL_RHI_OPENGL)
		    LinkShaders_OPENGL(material);
#endif
		    // TODO: Add other shader types
		    return result;
	  }
	
	  void Renderer::Draw(u32 vertexCount, u32 startVertex)
	  {
#if defined(CGL_RHI_DX11)
		    Draw_D3D11(vertexCount, startVertex);
#elif defined(CGL_RHI_OPENGL)
		    Draw_OPENGL(vertexCount, startVertex);
#endif
    }

    void Renderer::DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex)
    {
#if defined(CGL_RHI_DX11)
		    DrawIndexed_D3D11(indexCount, startIndex, baseVertex);
#elif defined(CGL_RHI_OPENGL)
		    DrawIndexed_OPENGL(indexCount, startIndex, baseVertex);
#endif
    }
}  // namespace CGL::Graphics