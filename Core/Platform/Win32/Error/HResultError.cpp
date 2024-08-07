#pragma once
#include <Core/Common.h>
#include <Windows.h>

namespace CGL::Platform::Win32
{
    std::string GetErrorString(int errorCode)
    {
        char errorString[MAX_PATH];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                         errorString, MAX_PATH, NULL);

        std::string message = "Win32 Error: ";
        message += errorString;
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());
        message.erase(std::remove(message.begin(), message.end(), '\r'), message.cend());
        return message;
    }
}  // namespace CGL::Platform::Win32
