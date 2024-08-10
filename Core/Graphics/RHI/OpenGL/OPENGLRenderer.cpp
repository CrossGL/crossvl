#include <Core/Graphics/RHI/OpenGL/OPENGLRendererImpl.h>
#include <Core/Graphics/Renderer.h>

namespace CGL::Graphics
{
#if defined(CGL_RHI_OPENGL)

    namespace Mapping
    {
        static constexpr std::array PrimitiveTopology =
        {
            GL_TRIANGLES,
            GL_LINES,
            GL_POINTS,
            GL_TRIANGLE_STRIP,
            GL_LINE_STRIP
        };

        static constexpr std::array BufferUsage =
        {
            GL_STATIC_DRAW,
            GL_DYNAMIC_DRAW,
            GL_STREAM_DRAW,
            GL_STREAM_READ            
        };

        static_assert(PrimitiveTopology.size() == size_t(PrimitiveType::COUNT));
        static_assert(BufferUsage.size() == size_t(BufferUsage::COUNT));
    }
    
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
        return static_cast<OPENGLRendererImpl*>(m_impl);
    }

    void Renderer::BeginFrame_OPENGL()
    {
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::EndFrame_OPENGL()
    {
        GetImpl()->SwapWindow();
    }

    void Renderer::Resize_OPENGL(u32 width, u32 height)
    {
        glViewport(0, 0, width, height);
    }

    void Renderer::SetPrimitiveTopology_OPENGL(PrimitiveType topology)
    {
        assert(GetImpl());
        GetImpl()->SetPrimitive(Mapping::PrimitiveTopology[size_t(topology)]);
    }

    void Renderer::SetVertexShader_OPENGL([[maybe_unused]] const VertexShader& shader)
    {
        assert(GetImpl());
    }

    void Renderer::SetPixelShader_OPENGL( [[maybe_unused]] const PixelShader& shader)
    {
        assert(GetImpl() );
    }

    void Renderer::SetVertexBuffer_OPENGL(const VertexBuffer& buffer)
    {
        assert(GetImpl());
        glBindVertexArray(buffer.VAO);
    }

    void Renderer::SetIndexBuffer_OPENGL([[maybe_unused]] const IndexBuffer& buffer)
    {
        assert(GetImpl());
    }

    ShaderCompileResult Renderer::CompileVertexShader_OPENGL(const ShaderSource& source, VertexShader* outShader)
    {
        assert(GetImpl() && outShader);

        CompileConfig cfg{};
        cfg.EntryPoint = "main";
#ifdef CGL_BUILD_DEBUG
        cfg.Debug = true;
        cfg.Optimize = false;
#endif

        ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->vertexShader);
        if (result.Status != ShaderCompileStatus::Failure)
        {
            assert(outShader->vertexShader);
        }

        return result;
    }

    ShaderCompileResult Renderer::CompilePixelShader_OPENGL(const ShaderSource& source, PixelShader* outShader)
    {
        assert(GetImpl() && outShader);

        CompileConfig cfg{};
        cfg.EntryPoint = "main";
#ifdef CGL_BUILD_DEBUG
        cfg.Debug = true;
        cfg.Optimize = false;
#endif

        ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->fragmentShader);
        if (result.Status != ShaderCompileStatus::Failure)
        {
            assert(outShader->fragmentShader);
        }

        return result;
    }

    void Renderer::LinkShaders_OPENGL(Material* material)
    {
        material->m_id = glCreateProgram();
        glAttachShader(material->m_id, material->GetVertexShader()->Shader.vertexShader);
        glAttachShader(material->m_id, material->GetPixelShader()->Shader.fragmentShader);
        glLinkProgram(material->m_id);

        GLint success;
        char infoLog[512];
        glGetProgramiv(material->m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(material->m_id, 512, nullptr, infoLog);
            CGL_LOG(Renderer, Error, "Failed to link shaders : " + std::string(infoLog));
        }
        else
        {
			      material->m_vs->State = ShaderState::Compiled;            
			      material->m_ps->State = ShaderState::Compiled;
            glDeleteShader(material->GetVertexShader()->Shader.vertexShader);
            glDeleteShader(material->GetPixelShader()->Shader.fragmentShader);
        }
    }

    VertexBuffer Renderer::CreateVertexBuffer_OPENGL(const BufferSource& source)
    {
        assert(source.Type == BufferType::Vertex);
        assert(GetImpl());

        VertexBuffer vb;
        glGenVertexArrays(1, &vb.VAO);
        glGenBuffers(1, &vb.VBO);
        glBindVertexArray(vb.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vb.VBO);
        glBufferData(GL_ARRAY_BUFFER, source.TypeSize * source.Count, source.Data, GL_STATIC_DRAW);

        // Position Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, source.TypeSize, (void*)0);
        glEnableVertexAttribArray(0);

        // Color Attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, source.TypeSize, (void*)(sizeof(SM::Vector3)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        return vb;
    }

    IndexBuffer Renderer::CreateIndexBuffer_OPENGL( [[maybe_unused]] const BufferSource& source)
    {
        return IndexBuffer();
    }

	void Renderer::CreateConstantBuffer_OPENGL([[maybe_unused]] const BufferSource& source, [[maybe_unused]] GLuint& buffer){}
	
    void Renderer::SetConstantBufferData_OPENGL([[maybe_unused]] GLuint* buffer, [[maybe_unused]] const void* data, [[maybe_unused]] size_t size){}

 	void Renderer::SetConstantBuffer_OPENGL([[maybe_unused]] ShaderType type, [[maybe_unused]] u32 startSlot, [[maybe_unused]] const GLuint& buffer){}

    void Renderer::Draw_OPENGL(u32 vertexCount, u32 startVertex)
    {
        assert(GetImpl());
        glDrawArrays(GetImpl()->GetPrimitive(), startVertex, vertexCount);
    }

    void Renderer::DrawIndexed_OPENGL([[maybe_unused]] u32 indexCount, [[maybe_unused]] u32 startIndex, [[maybe_unused]] u32 baseVertex)
    {
        assert(GetImpl());
    }

#endif // CGL_RHI_OPENGL
}