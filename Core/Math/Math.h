#pragma once

// Including these to enable API conversion functions
#if defined(CGL_RHI_DX11)
    #include <d3d11.h>
#elif defined(CGL_RHI_DX12)
    #include <d3d12.h>
#endif

#include <Core/External/SimpleMath.h>
#include <DirectXColors.h>

using namespace DirectX;
namespace SM = DirectX::SimpleMath;
