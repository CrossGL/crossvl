#pragma once

#include "Metal/MTLBuffer.hpp"

namespace CGL::Graphics
{
    template <class T>
    struct METALConstantBuffer
    {
        using value_type = T;

        METALConstantBuffer()
        {
            static_assert((sizeof(T) % 16) == 0, "Constant buffer size must be 16-byte aligned.");
        }

        MTL::Buffer* Buffer;
    };
}
