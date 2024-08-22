#pragma once

#include "Metal/MTLLibrary.hpp"

namespace CGL::Graphics
{
    struct METALPixelShader
    {
        std::unique_ptr<MTL::Library*> SourceContent;
        MTL::Function* Shader;
    };
}
