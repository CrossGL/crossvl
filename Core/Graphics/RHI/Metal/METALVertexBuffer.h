#pragma once

#include "Foundation/NSTypes.hpp"
#include "Metal/MTLBuffer.hpp"

namespace CGL::Graphics
{
    struct METALVertexBuffer
    {
        MTL::Buffer* Buffer;

        NS::UInteger Offset;
        NS::UInteger Index;
    };
}
