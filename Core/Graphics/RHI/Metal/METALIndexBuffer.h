#pragma once

#include "Foundation/NSTypes.hpp"
#include "Metal/MTLBuffer.hpp"
#include "Metal/MTLStageInputOutputDescriptor.hpp"

namespace CGL::Graphics
{
    struct METALIndexBuffer
    {
        MTL::Buffer* Buffer;
        MTL::IndexType Format;

        NS::UInteger IndicesCount;
    };
}
