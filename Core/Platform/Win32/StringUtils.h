#pragma once
#include <Windows.h>
#include <string>

namespace CGL::Core::Platform::Win32
{
    inline std::wstring ToWideStr(const std::string& str) noexcept
    {
        int cch = MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), nullptr, 0);

        std::wstring wideStr;
        wideStr.resize(cch);
        ::MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), wideStr.data(), cch);

        return wideStr;
    }

    inline std::string ToNarrowStr(const std::wstring& wstr) noexcept
    {
        int cch =
            WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);

        std::string narrowStr;
        narrowStr.resize(cch);
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), &narrowStr[0], cch, nullptr,
                            nullptr);

        return narrowStr;
    }
}  // namespace CGL::Core::Platform::Win32
