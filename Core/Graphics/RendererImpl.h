#pragma once

#if defined(CGL_RHI_DX11) + defined(CGL_RHI_DX12) + defined(CGL_RHI_OPENGL) + defined(CGL_RHI_VULKAN) + defined(CGL_RHI_METAL) != 1
#error "Illegal to build for multiple API's. Exactly one of CGL_RHI_DX11, CGL_RHI_DX12, CGL_RHI_OPENGL, CGL_RHI_VULKAN or CGL_RHI_METAL must be defined"
#endif


#if defined(CGL_RHI_DX11)
#include <Core/Graphics/RHI/D3D11/D3D11RendererImpl.h>
#elif defined(CGL_RHI_DX12)
#include <Core/Graphics/RHI/D3D12/D3D12RendererImpl.h>
#elif defined(CGL_RHI_OPENGL)
#include <Core/Graphics/RHI/OpenGL/OPENGLRendererImpl.h>
#elif defined(CGL_RHI_VULKAN)
#include <Core/Graphics/RHI/Vulkan/VULKANRendererImpl.h>
#elif defined(CGL_RHI_METAL)
#include <Core/Graphics/RHI/Metal/METALRendererImpl.h>
#endif
