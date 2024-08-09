#include "Log.h"
#include <Core/Logging/LogManager.h>
#include <Core/Utils/Singleton.h>
#include <sstream>

namespace CGL::Core
{
    void Log::Write(LogLevel level, const LogMessage& message)
    {
        constexpr std::string_view levelString[] = { "[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]" };
        i8 levelValue                            = i8(level);

        std::string outMsg = std::format("{} [{}] {}\n", levelString[levelValue], m_name, message.Payload);

        std::scoped_lock lock{ m_mutex };

        Utils::Singleton<LogManager>::Get().Write(level, outMsg);
    }
}  // namespace CGL::Core
