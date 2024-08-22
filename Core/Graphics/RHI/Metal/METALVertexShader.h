#pragma once

#include "Metal/MTLLibrary.hpp"

namespace CGL::Graphics
{
    struct METALVertexShader
    {
        std::unique_ptr<MTL::Library*> SourceContent;
        MTL::Function* Shader;
    };
}
