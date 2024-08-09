#pragma once
#include <Core/Graphics/RHI/D3DCommon.h>

namespace CGL::Graphics
{
    struct D3D11VertexBuffer
    {
        ComPtr<ID3D11Buffer> Buffer;
        u32 Stride;
        u32 Offset;
    };
}  // namespace CGL::Graphics
