#pragma once
#include <Core/Common.h>

namespace CGL::Graphics
{
    enum class ShaderCompileStatus
    {
        Success,
        Failure,
        HasWarnings
    };

    struct ShaderCompileResult
    {
        ShaderCompileStatus Status;
        std::string Message;
    };
}  // namespace CGL::Graphics
