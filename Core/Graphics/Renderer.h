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

	RHIType GetAPI();

	class Renderer
	{
	public:
		explicit Renderer(SDL_Window* window, RHIType api);
		~Renderer();

		void BeginFrame();
		void EndFrame();

		void Resize(u32 width, u32 height);

	private:
#ifdef CGL_RHI_DX11
		void Constructor_D3D11();
		void Destructor_D3D11();
		void BeginFrame_D3D11();
		void EndFrame_D3D11();
		void Resize_D3D11(u32 width, u32 height);
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

	private:
		void* m_impl;
	};
}
