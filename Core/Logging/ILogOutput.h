#pragma once
#include <Core/Logging/LogLevel.h>

namespace CGL::Core
{
    // Abstract class for log outputs
    class ILogOutput
    {
    public:
        virtual ~ILogOutput()                                                           = default;
        virtual void Write([[maybe_unused]] LogLevel level, const std::string& message) = 0;
    };
}  // namespace CGL::Core
