#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
    struct D3D11PixelShader
    {
        ComPtr<ID3D11PixelShader> Shader;
        ComPtr<ID3DBlob> Blob;
    };
}  // namespace CGL::Graphics
