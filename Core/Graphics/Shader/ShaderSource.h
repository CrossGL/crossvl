#pragma once
#include <Core/Common.h>
#include <Core/Graphics/Types.h>

namespace CGL::Graphics
{
    struct ShaderSource
    {
        ShaderType Type;
        std::string SourceData;
        std::string Name;
    };
}  // namespace CGL::Graphics
