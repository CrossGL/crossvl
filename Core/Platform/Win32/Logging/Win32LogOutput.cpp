#include "Win32LogOutput.h"
#include <array>

namespace CGL::Core::Platform::Win32
{
    namespace
    {
        void SetConsoleForegroundColor(HANDLE console, u16 color)
        {
            CONSOLE_SCREEN_BUFFER_INFO info = {};
            if (GetConsoleScreenBufferInfo(console, &info))
            {
                SetConsoleTextAttribute(console, (info.wAttributes & 0xF0) | color);
            }
        }

        u16 GetConsoleTextAttribute(HANDLE console)
        {
            CONSOLE_SCREEN_BUFFER_INFO info = {};
            if (GetConsoleScreenBufferInfo(console, &info))
            {
                return info.wAttributes;
            }
            return 0;
        }
    }  // namespace

    ConsoleLogOutput::ConsoleLogOutput(std::string_view name)
    {
        // Try getting the attached console if it already exists
        m_console = ::GetStdHandle(STD_OUTPUT_HANDLE);
        if (m_console == INVALID_HANDLE_VALUE)
        {
            // No attached console found, attach
            ::AllocConsole();
        }

        ::SetConsoleTitleA(name.data());
        m_console = ::GetStdHandle(STD_OUTPUT_HANDLE);
        COORD co  = { 80, 25 };
        ::SetConsoleScreenBufferSize(m_console, co);
    }

    ConsoleLogOutput::~ConsoleLogOutput()
    {
        ::FreeConsole();
    }

    void ConsoleLogOutput::Write(LogLevel level, const std::string& message)
    {
        static constexpr std::array<u16, size_t(LogLevel::NUM_COUNT)> colors = {
            FOREGROUND_RED | FOREGROUND_INTENSITY,                     // Fatal
            FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,   // Error
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,  // Warn
            FOREGROUND_GREEN,                                          // Info
            FOREGROUND_GREEN | FOREGROUND_BLUE,                        // Debug
            FOREGROUND_INTENSITY,                                      // Trace
        };

        // Sanity check
        if (m_console == INVALID_HANDLE_VALUE)
        {
            return;
        }

        u16 originalColor = GetConsoleTextAttribute(m_console);
        SetConsoleForegroundColor(m_console, colors[size_t(level)]);

        std::ignore = ::WriteConsoleA(m_console, message.data(), static_cast<DWORD>(message.size()), nullptr, nullptr);
        // Restore color
        SetConsoleForegroundColor(m_console, originalColor);
    }

    void DebugLogOutput::Write(LogLevel level, const std::string& message)
    {
        UNREFERENCED_PARAMETER(level);
        ::OutputDebugStringA(message.c_str());
    }

    FileLogOutput::FileLogOutput(std::string_view name)
        : m_name(name), m_file(nullptr)
    {
        m_file =
            ::CreateFileA(m_name.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        Write(LogLevel::Info, "Starting log file: " + m_name + "\n");
    }

    FileLogOutput::~FileLogOutput()
    {
        if (m_file != nullptr)
        {
            ::FlushFileBuffers(m_file);
            ::CloseHandle(m_file);
        }
    }

    void FileLogOutput::Write(LogLevel level, const std::string& message)
    {
        // Sanity check
        if (m_file == nullptr)
        {
            return;
        }

        UNREFERENCED_PARAMETER(level);
        std::ignore = ::WriteFile(m_file, message.data(), static_cast<DWORD>(message.size()), nullptr, nullptr);
    }

    CallbackLogOutput::CallbackLogOutput(std::function<void(LogLevel, const std::string&)> callback)
        : m_callback(std::move(callback))
    {
    }

    void CallbackLogOutput::Write(LogLevel level, const std::string& message)
    {
        m_callback(level, message);
    }
}  // namespace CGL::Core::Platform::Win32
