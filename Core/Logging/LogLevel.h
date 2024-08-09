#pragma once
#include <Core/Common.h>

namespace CGL::Core
{
    enum class LogLevel : i8
    {
        Fatal = 0,
        Error = 1,
        Warn  = 2,
        Info  = 3,
        Debug = 4,
        Trace = 5,

        NUM_COUNT
    };
}
