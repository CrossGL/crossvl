#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Types.h>
#include <Core/Logging/Log.h>
#include <Core/Graphics/Shader/Shader.h>
#include <Core/Graphics/Shader/ShaderCompiler.h>

struct SDL_Window;

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(Renderer);

#if defined(CGL_RHI_DX11)
	class D3D11RendererImpl;
#elif defined(CGL_RHI_DX12)
	class D3D12RendererImpl;
#elif defined(CGL_RHI_OPENGL)
	class OPENGLRendererImpl;
#elif defined(CGL_RHI_METAL)
	class METALRendererImpl;
#elif defined(CGL_RHI_VULKAN)
	class VULKANRendererImpl;
#endif

	RHIType GetAPI();

	class Renderer
	{
	public:
		explicit Renderer(SDL_Window* window, RHIType api);
		~Renderer();

		void BeginFrame();
		void EndFrame();

		void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
		void SetPrimitiveTopology(PrimitiveType topology);
		void Resize(u32 width, u32 height);

		std::shared_ptr<VertexShader> CreateVertexShader(const ShaderSource& source);
		std::shared_ptr<PixelShader> CreatePixelShader(const ShaderSource& source);

	private:
#ifdef CGL_RHI_DX11
		void Constructor_D3D11(SDL_Window* window);
		void Destructor_D3D11();
		void BeginFrame_D3D11();
		void EndFrame_D3D11();
		void Resize_D3D11(u32 width, u32 height);
		void SetPrimitiveTopology_D3D11(PrimitiveType topology);
		ShaderCompileResult CreateVertexShader_D3D11(const ShaderSource& source, std::shared_ptr<VertexShader>& outShader);
		D3D11RendererImpl* GetImpl() const;
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		void Constructor_OPENGL(SDL_Window* window);
		void Destructor_OPENGL();
		void BeginFrame_OPENGL();
		void EndFrame_OPENGL();
		void Resize_OPENGL(u32 width, u32 height);
		OPENGLRendererImpl* GetImpl() const;
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
		void Constructor_METAL(SDL_Window* window);
		void Destructor_METAL();
		void BeginFrame_METAL();
		void EndFrame_METAL();
		void Resize_METAL(u32 width, u32 height);
		METALRendererImpl* GetImpl() const;
#endif

	private:
		void* m_impl;
		std::array<f32, 4> m_clearColor;
		u32 m_width;
		u32 m_height;
	};
}
