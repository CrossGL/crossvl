#include "LogManager.h"

namespace CGL::Core
{
    LogManager::~LogManager()
    {
        m_outputs.clear();
    }

    void LogManager::Write(LogLevel level, const std::string& message)
    {
        for (auto& [name, output] : m_outputs)
        {
            output->Write(level, message);
        }
    }

    const std::unordered_map<std::string, std::unique_ptr<ILogOutput>>& LogManager::GetOutputs() const noexcept
    {
        return m_outputs;
    }
}  // namespace CGL::Core
