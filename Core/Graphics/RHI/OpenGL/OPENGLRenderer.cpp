#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/RHI/OpenGL/OPENGLRendererImpl.h>
#include <Core/Graphics/RHI/OpenGL/OPENGLVertexAttributes.h>

namespace CGL::Graphics
{
#if defined(CGL_RHI_OPENGL)

    namespace Mapping
    {
			static constexpr std::array<GLenum, 5> PrimitiveTopology =
			{
				GL_TRIANGLES,
				GL_LINES,
				GL_POINTS,
				GL_TRIANGLE_STRIP,
				GL_LINE_STRIP
			};

			static constexpr std::array<GLenum, 4> BufferUsage =
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
			glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Renderer::EndFrame_OPENGL()
    { 
			GetImpl()->Present();
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
			// This function is currently empty because shader creation and compilation are handled
			// by separate functions. The SetVertexShader function will be implemented to switch to 
			// an active vertex shader for each frame as needed.
			assert(GetImpl());
	}

	void Renderer::SetPixelShader_OPENGL( [[maybe_unused]] const PixelShader& shader)
	{
			// This function is currently empty because shader creation and compilation are handled
			// by separate functions. The SetPixelShader function will be implemented to switch to 
			// an active pixel shader for each frame as needed.
			assert(GetImpl() );
	}

	void Renderer::SetVertexBuffer_OPENGL(const VertexBuffer& buffer)
	{
			assert(GetImpl());
			glBindVertexArray(buffer.VAO);
	}

	void Renderer::SetIndexBuffer_OPENGL( const IndexBuffer& buffer)
	{
			assert(GetImpl());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
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

			ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->VertexShader);
			if (result.Status != ShaderCompileStatus::Failure)
			{
				assert(outShader->VertexShader);
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

			ShaderCompileResult result = ShaderCompiler::Compile(source, cfg, outShader->PixelShader);
			if (result.Status != ShaderCompileStatus::Failure)
			{
				assert(outShader->PixelShader);
			}

			return result;
	}

	void Renderer::LinkShaders_OPENGL(Material* material)
	{
			material->m_id = glCreateProgram();
			glAttachShader(material->m_id, material->GetVertexShader()->Shader.VertexShader);
			glAttachShader(material->m_id, material->GetPixelShader()->Shader.PixelShader);
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
				glDeleteShader(material->GetVertexShader()->Shader.VertexShader);
				glDeleteShader(material->GetPixelShader()->Shader.PixelShader);
			}
	}

	VertexBuffer Renderer::CreateVertexBuffer_OPENGL(const BufferSource& source)
	{
			assert(source.Type == BufferType::Vertex);
			assert(GetImpl());

			VertexBuffer vb;
			vb.Size = source.TypeSize * source.Count;
			glGenVertexArrays(1, &vb.VAO);
			glGenBuffers(1, &vb.VBO);
			glBindVertexArray(vb.VAO);

			glBindBuffer(GL_ARRAY_BUFFER, vb.VBO);
			glBufferData(GL_ARRAY_BUFFER, vb.Size, source.Data, Mapping::BufferUsage[size_t(source.Usage)]);

			if (source.VertexType == typeid(VertexTypes::Position))
			{
				OPENGLEnableVertexAttributes<VertexTypes::Position>();
			}
			else if (source.VertexType == typeid(VertexTypes::PositionColor))
			{
				OPENGLEnableVertexAttributes<VertexTypes::PositionColor>();
			}
			glBindVertexArray(0);       

			return vb;
	}

	IndexBuffer Renderer::CreateIndexBuffer_OPENGL(const BufferSource& source)
	{
			assert(source.Type == BufferType::Index);
			assert(GetImpl());

			IndexBuffer ib;
			ib.Size = source.TypeSize * source.Count;
			ib.IndicesCount = source.Count;

			glGenBuffers(1, &ib.EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib.Size, source.Data, Mapping::BufferUsage[size_t(source.Usage)]);

			return ib;
	}

	void Renderer::CreateConstantBuffer_OPENGL(const BufferSource& source, GLuint& outBuffer)
	{
			assert(source.Type == BufferType::Constant);
			assert(GetImpl());

			glGenBuffers(1, &outBuffer);

			glBindBuffer(GL_UNIFORM_BUFFER, outBuffer);
			glBufferData(GL_UNIFORM_BUFFER, source.TypeSize, nullptr, Mapping::BufferUsage[size_t(source.Usage)]);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, outBuffer);
	}

	void Renderer::SetConstantBufferData_OPENGL(const GLuint& buffer, const void* data, size_t size)
	{
			assert(buffer);
			glBindBuffer(GL_UNIFORM_BUFFER, buffer);
			
			glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Renderer::SetConstantBuffer_OPENGL([[maybe_unused]] ShaderType type, [[maybe_unused]] u32 startSlot, [[maybe_unused]] const GLuint& buffer)
	{
			// This function is currently empty because uniform buffer binding is managed directly
			// by the shader's 'binding' layout qualifier. If runtime shader switching or dynamic
			// buffer management is needed, this function will handle the necessary binding logic.
			assert(buffer);
	}

	void Renderer::Draw_OPENGL(u32 vertexCount, u32 startVertex)
	{
			assert(GetImpl());
			glDrawArrays(GetImpl()->GetPrimitive(), startVertex, vertexCount);
	}

	void Renderer::DrawIndexed_OPENGL(u32 indexCount, u32 startIndex, u32 baseVertex)
	{
			assert(GetImpl());
			glDrawElementsBaseVertex(GetImpl()->GetPrimitive(), indexCount, GL_UNSIGNED_INT, (void*)(startIndex * sizeof(GLuint)), baseVertex);
	}

#endif // CGL_RHI_OPENGL
}