#pragma once
#include <Core/Common.h>
#include <Core/Logging/ILogOutput.h>
#include <Windows.h>

namespace CGL::Core::Platform::Win32
{
    class ConsoleLogOutput : public ILogOutput
    {
    public:
        explicit ConsoleLogOutput(std::string_view name);
        ~ConsoleLogOutput();
        void Write([[maybe_unused]] LogLevel level, const std::string& message) override;

    private:
        HANDLE m_console;
    };

    class DebugLogOutput : public ILogOutput
    {
    public:
        DebugLogOutput() = default;
        void Write([[maybe_unused]] LogLevel level, const std::string& message) override;
    };

    class FileLogOutput : public ILogOutput
    {
    public:
        explicit FileLogOutput(std::string_view name);
        ~FileLogOutput();
        void Write([[maybe_unused]] LogLevel level, const std::string& message) override;

    private:
        std::string m_name;
        HANDLE m_file;
    };

    class CallbackLogOutput : public ILogOutput
    {
    public:
        explicit CallbackLogOutput(std::function<void(LogLevel, const std::string&)> callback);
        void Write([[maybe_unused]] LogLevel level, const std::string& message) override;

    private:
        std::function<void(LogLevel, const std::string&)> m_callback;
    };
}  // namespace CGL::Core::Platform::Win32
