#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Types.h>
#include <Core/Logging/Log.h>

struct SDL_Window;

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(Renderer);

#ifdef CGL_RHI_DX11
	class D3D11RendererImpl;
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
	class OPENGLRendererImpl;
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
	class METALRendererImpl;
#endif

	RHIType GetAPI();

	class Renderer
	{
	public:
		explicit Renderer(SDL_Window* window, RHIType api);
		~Renderer();

		void BeginFrame();
		void OnRender();
		void EndFrame();

		void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
		void SetPrimitiveTopology(PrimitiveType topology);
		void Resize(u32 width, u32 height);

	private:
#ifdef CGL_RHI_DX11
		void Constructor_D3D11(SDL_Window* window);
		void Destructor_D3D11();
		void BeginFrame_D3D11();
		void EndFrame_D3D11();
		void Resize_D3D11(u32 width, u32 height);
		void SetPrimitiveTopology_D3D11(PrimitiveType topology);
		D3D11RendererImpl* GetImpl() const;
#endif // CGL_RHI_DX11

#ifdef CGL_RHI_OPENGL
		void Constructor_OPENGL();
		void Destructor_OPENGL();
		void BeginFrame_OPENGL();
		void EndFrame_OPENGL();
		void Resize_OPENGL(u32 width, u32 height);
		OPENGLRendererImpl* GetImpl() const;
#endif // CGL_RHI_OPENGL

#ifdef CGL_RHI_METAL
<<<<<<< HEAD
		  void Constructor_METAL(SDL_Window* window);
		  void Destructor_METAL();
		  void BeginFrame_METAL();
		  void OnRender_METAL();
		  void EndFrame_METAL();
		  void Resize_METAL(u32 width, u32 height);
		  METALRendererImpl* GetImpl() const;
=======
		void Constructor_METAL(SDL_Window* window);
		void Destructor_METAL();
		void BeginFrame_METAL();
		void OnRender_METAL();
		void EndFrame_METAL();
		void Resize_METAL(u32 width, u32 height);
		METALRendererImpl* GetImpl() const;
>>>>>>> c83f60b (implemented RHI for Metal)
#endif

	private:
		void* m_impl;
		std::array<f32, 4> m_clearColor;
		u32 m_width;
		u32 m_height;
	};
}
