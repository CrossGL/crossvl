#pragma once
#if defined(CGL_RHI_DX11)
#include <Core/Graphics/RHI/D3D11/D3D11VertexShader.h>
#include <Core/Graphics/RHI/D3D11/D3D11PixelShader.h>
#endif

#if defined(CGL_RHI_DX12)
#include <Core/Graphics/RHI/D3D12/D3D12VertexShader.h>
#endif

#if defined(CGL_RHI_OPENGL)
#include <Core/Graphics/RHI/OpenGL/OPENGLVertexShader.h>
#include <Core/Graphics/RHI/OpenGL/OPENGLPixelShader.h>
#endif

#if defined(CGL_RHI_METAL)
#include <Core/Graphics/RHI/Metal/METALVertexShader.h>
#endif

#if defined(CGL_RHI_VULKAN)
#include <Core/Graphics/RHI/Vulkan/VULKANVertexShader.h>
#endif

namespace CGL::Graphics
{
#if defined(CGL_RHI_DX11)
	using VertexShader = D3D11VertexShader;
	using PixelShader  = D3D11PixelShader;
#elif defined(CGL_RHI_DX12)
	using VertexShader = D3D12VertexShader;
	using PixelShader  = D3D12PixelShader;
#elif defined(CGL_RHI_OPENGL)
	using VertexShader = OPENGLVertexShader;
	using PixelShader  = OPENGLPixelShader;
#elif defined(CGL_RHI_METAL)
	using VertexShader = METALVertexShader;
	using PixelShader  = METALPixelShader;
#elif defined(CGL_RHI_VULKAN)
	using VertexShader = VULKANVertexShader;
	using PixelShader  = VULKANPixelShader;
#else
	#error Unsupported shader RHI
#endif
}
