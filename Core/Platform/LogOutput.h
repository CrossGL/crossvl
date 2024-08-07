#pragma once
#ifdef CGL_PLATFORM_WINDOWS
    #include <Core/Platform/Win32/Logging/Win32LogOutput.h>
#elif CGL_PLATFORM_LINUX
    #include <Core/Platform/Linux/Logging/LinuxLogOutput.h>
#elif CGL_PLATFORM_MACOSX
    #include <Core/Platform/MacOSX/Logging/MacOSXLogOutput.h>
#else
    #error "LogOutput not supported on this platform"
#endif

namespace CGL::Core
{
#ifdef CGL_PLATFORM_WINDOWS
    using ConsoleLogOutput  = Platform::Win32::ConsoleLogOutput;
    using DebugLogOutput    = Platform::Win32::DebugLogOutput;
    using FileLogOutput     = Platform::Win32::FileLogOutput;
    using CallbackLogOutput = Platform::Win32::CallbackLogOutput;
#elif CGL_PLATFORM_LINUX
    using ConsoleLogOutput  = Platform::Linux::ConsoleLogOutput;
    using DebugLogOutput    = Platform::Linux::DebugLogOutput;
    using FileLogOutput     = Platform::Linux::FileLogOutput;
    using CallbackLogOutput = Platform::Linux::CallbackLogOutput;
#elif CGL_PLATFORM_MACOSX
    using ConsoleLogOutput  = Platform::MacOSX::ConsoleLogOutput;
    using DebugLogOutput    = Platform::MacOSX::DebugLogOutput;
    using FileLogOutput     = Platform::MacOSX::FileLogOutput;
    using CallbackLogOutput = Platform::MacOSX::CallbackLogOutput;
#else
    #error "No log output implementation for this platform"
#endif
}  // namespace CGL::Core
