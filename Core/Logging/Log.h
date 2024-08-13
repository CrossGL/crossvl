#pragma once
#include <Core/Logging/LogLevel.h>
#include <mutex>

namespace CGL::Core
{
    struct LogMessage
    {
        std::string_view Payload;
    };

    class Log
    {
    public:
        explicit Log(std::string_view name)
            : m_name(name)
        {
        }

        template <typename T, typename... Args>
        void Trace(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Trace, message, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void Debug(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Debug, message, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void Info(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Info, message, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void Warn(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Warn, message, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void Error(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Error, message, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void Fatal(const T& message, Args&&... args)
        {
            DispatchLog(LogLevel::Fatal, message, std::forward<Args>(args)...);
        }

    private:
        template <typename... Args>
        void DispatchLog(LogLevel level, std::string_view message, Args&&... args)
        {
            std::string msg = std::vformat(std::string(message), std::make_format_args(std::forward<Args>(args)...));
            Write(level, LogMessage{ .Payload = std::string_view(msg.data(), msg.size()) });
        }

        void Write(LogLevel level, const LogMessage& message);

    private:
        std::string_view m_name;
        std::mutex m_mutex;
    };

}  // namespace CGL::Core

#define LOG_CONCATENATE(a, b) a##b

#define CGL_DECLARE_LOG_CATEGORY(Name)              \
    extern struct Log##Name : public CGL::Core::Log \
    {                                               \
        Log##Name()                                 \
            : Log(#Name)                            \
        {                                           \
        }                                           \
    } LOG_CONCATENATE(g_Log, Name)

#define CGL_DECLARE_LOG_CATEGORY_INLINE(Name) \
    struct Log##Name : public CGL::Core::Log  \
    {                                         \
        Log##Name()                           \
            : Log(#Name)                      \
        {                                     \
        }                                     \
    } LOG_CONCATENATE(g_Log, Name)

#define CGL_DEFINE_LOG_CATEGORY(Name) Log##Name LOG_CONCATENATE(g_Log, Name)

#ifdef CGL_LOGGING_ENABLED
    #define CGL_LOG(Name, Level, ...) LOG_CONCATENATE(g_Log, Name).Level(__VA_ARGS__)
#else
    #define CGL_LOG(Name, Level, ...) (void)0
#endif

#if CGL_BUILD_DEBUG
    #define CGL_LOG_DEBUG(Name, Level, ...) CGL_LOG(Name, Level, __VA_ARGS__)
#else
    #define CGL_LOG_DEBUG(Name, ...) (void)0
#endif
