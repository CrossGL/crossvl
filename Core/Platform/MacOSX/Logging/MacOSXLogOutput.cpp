#include "MacOSXLogOutput.h"
#include "curses.h"

namespace CGL::Core::Platform::MacOSX
{
    ConsoleLogOutput::ConsoleLogOutput(std::string_view name)
    {
        initscr();
        if (has_colors() == FALSE)
        {
            endwin();
            printf("Your terminal does not support color\n");
            exit(1);
        }
        start_color();

        init_pair(0, COLOR_RED, COLOR_BLACK);                  // Fatal
        init_pair(1, COLOR_RED | COLOR_BLUE, COLOR_BLACK);     // Error
        init_pair(2, COLOR_GREEN | COLOR_BLACK, COLOR_BLACK);  // Warn
        init_pair(3, COLOR_GREEN | COLOR_BLACK, COLOR_BLACK);  // Info
        init_pair(4, COLOR_GREEN | COLOR_BLUE, COLOR_BLACK);   // Debug
        init_pair(5, COLOR_WHITE, COLOR_BLACK);                // Trace

        attron(COLOR_PAIR(4));
        printw("%s\n", name.data());
        attron(COLOR_PAIR(4));
    }

    ConsoleLogOutput::~ConsoleLogOutput()
    {
        endwin();
    }

    void ConsoleLogOutput::Write([[maybe_unused]] LogLevel level, const std::string& message)
    {
        attron(COLOR_PAIR(level));
        printw(message.c_str());
        attroff(COLOR_PAIR(level));
        refresh();
    }

    FileLogOutput::FileLogOutput(std::string_view name)
        : m_name{ name }
    {
        file.open(std::string(m_name), std::ios::out | std::ios::trunc);

        if (file.is_open())
        {
            attron(COLOR_PAIR(5));
            printw("Starting Log File - %s\n", name.data());
            attroff(COLOR_PAIR(5));
        }
        else
        {
            attron(COLOR_PAIR(1));
            printw("Failed to Open File\n");
            attroff(COLOR_PAIR(1));
        }
    }

    FileLogOutput::~FileLogOutput()
    {
        file.close();
    }

    void FileLogOutput::Write([[maybe_unused]] LogLevel level, const std::string& message)
    {
        if (file.is_open())
        {
            file << message;
            file.flush();
        }
    }

    CallbackLogOutput::CallbackLogOutput(std::function<void(LogLevel, const std::string&)> callback)
        : m_callback(std::move(callback))
    {
    }

    void CallbackLogOutput::Write(LogLevel level, const std::string& message)
    {
        m_callback(level, message);
    }
}  // namespace CGL::Core::Platform::MacOSX
