#include "Renderer.h"
#include "Core/Graphics/Types.h"
#include <Core/Graphics/RendererImpl.h>
#include <SDL2/SDL.h>

namespace CGL::Graphics
{
	CGL_DEFINE_LOG_CATEGORY(Renderer);

	namespace
	{
		RHIType g_api = RHIType::None;
	}

	RHIType GetAPI()
	{
		return g_api;
	}

	Renderer::Renderer(SDL_Window* window, RHIType api)
		: m_impl(nullptr)
		, m_clearColor({ 1.0f, 0.0f, 1.0f, 1.0f })
	{
		g_api = api;

#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			CGL_LOG(Renderer, Debug, "Using RHI: DirectX11");
			Constructor_D3D11(window);
			return;
		}
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		if (g_api == RHIType::OpenGL)
		{
			CGL_LOG(Renderer, Debug, "Using RHI: OpenGL");
			Constructor_OPENGL(window);
			return;
		}
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		if (g_api == RHIType::Metal)
		{
			CGL_LOG(Renderer, Debug, "Using RHI: Metal");
			Constructor_METAL(window);
			return;
		}
#endif

		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	Renderer::~Renderer()
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			CGL_LOG(Renderer, Info, "Destructing RHI: DirectX11");
			Destructor_D3D11();
			return;
		}
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		if (g_api == RHIType::OpenGL)
		{
			CGL_LOG(Renderer, Info, "Destructing RHI: OpenGL");
			Destructor_OPENGL();
			return;
		}
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		if (g_api == RHIType::Metal)
		{
			CGL_LOG(Renderer, Info, "Destructing RHI: Metal");
			Destructor_METAL();
			return;
		}
#endif

		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	void Renderer::BeginFrame()
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			BeginFrame_D3D11();
			return;
		}
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		if (g_api == RHIType::OpenGL)
		{
			BeginFrame_OPENGL();
			return;
		}
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		if (g_api == RHIType::Metal)
		{
			BeginFrame_METAL();
			return;
		}
#endif

		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	void Renderer::EndFrame()
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			EndFrame_D3D11();
			return;
		}
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		if (g_api == RHIType::OpenGL)
		{
			EndFrame_OPENGL();
			return;
		}
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		if (g_api == RHIType::Metal)
		{
			EndFrame_METAL();
			return;
		}
#endif

		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	void Renderer::Resize(u32 width, u32 height)
	{
		if (m_width == width && m_height == height)
		{
			return;
		}

		m_width = width;
		m_height = height;

#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			Resize_D3D11(width, height);
			return;
		}
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		if (g_api == RHIType::OpenGL)
		{
			Resize_OPENGL(width, height);
			return;
		}
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		if (g_api == RHIType::Metal)
		{
			Resize_METAL(width, height);
			return;
		}
#endif

		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	void Renderer::SetClearColor(f32 r, f32 g, f32 b, f32 a)
	{
		m_clearColor = { r, g, b, a };
	}

	void Renderer::SetPrimitiveTopology(PrimitiveType topology)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			SetPrimitiveTopology_D3D11(topology);
			return;
		}
#endif // CGL_RHI_DX11
		// We should always have an RHI, should never reach here
		std::unreachable();
	}

	void Renderer::SetVertexShader(const std::shared_ptr<VertexShader>& shader)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			SetVertexShader_D3D11(shader);
			return;
		}
#endif // CGL_RHI_DX11
	}

	void Renderer::SetPixelShader(const std::shared_ptr<PixelShader>& shader)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			SetPixelShader_D3D11(shader);
			return;
		}
#endif // CGL_RHI_DX11
	}

	void Renderer::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			SetVertexBuffer_D3D11(buffer);
			return;
		}
#endif // CGL_RHI_DX11
	}

	void Renderer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			SetIndexBuffer_D3D11(buffer);
			return;
		}
#endif // CGL_RHI_DX11
	}

	std::shared_ptr<VertexShader> Renderer::CreateVertexShader(const ShaderSource& source)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			std::shared_ptr<VertexShader> vs = std::make_shared<VertexShader>();
			ShaderCompileResult result = CreateVertexShader_D3D11(source, vs);

			ShaderCompiler::ReportResult(result, source.Name.data());

			// Return even if we have warnings
			return (result.Status != ShaderCompileStatus::Failure) ? std::move(vs) : nullptr;
		}
#endif // CGL_RHI_DX11
	}

	std::shared_ptr<PixelShader> Renderer::CreatePixelShader(const ShaderSource& source)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			std::shared_ptr<PixelShader> ps = std::make_shared<PixelShader>();
			ShaderCompileResult result = CreatePixelShader_D3D11(source, ps);

			ShaderCompiler::ReportResult(result, source.Name.data());

			// Return even if we have warnings
			return (result.Status != ShaderCompileStatus::Failure) ? std::move(ps) : nullptr;
		}
#endif // CGL_RHI_DX11
	}

	std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(const BufferSource& source)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			ID3D11Buffer* vb = CreateVertexBuffer_D3D11(source);
			return (vb == nullptr) ? nullptr : std::make_shared<VertexBuffer>(vb, source.Size, 0);
		}
#endif // CGL_RHI_DX11
	}

	std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(const BufferSource& source)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			ID3D11Buffer* ib = CreateIndexBuffer_D3D11(source);
			return (ib == nullptr) ? nullptr : std::make_shared<IndexBuffer>(ib);
		}
#endif // CGL_RHI_DX11
	}
	
	void Renderer::Draw(u32 vertexCount, u32 startVertex)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			Draw_D3D11(vertexCount, startVertex);
			return;
		}
#endif // CGL_RHI_DX11
	}

	void Renderer::DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex)
	{
#ifdef CGL_RHI_DX11
		if (g_api == RHIType::DirectX11)
		{
			DrawIndexed_D3D11(indexCount, startIndex, baseVertex);
			return;
		}
#endif // CGL_RHI_DX11
	}
}
