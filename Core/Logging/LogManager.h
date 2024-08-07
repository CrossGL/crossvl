#pragma once
#include <Core/Common.h>
#include <Core/Platform/LogOutput.h>
#include <unordered_map>

namespace CGL::Core
{
    class LogManager
    {
    public:
        LogManager() = default;
        ~LogManager();

        template <typename T, typename... Args>
        void AttachOutput(std::string_view name, Args&&... args)
        {
            m_outputs[std::string(name)] = std::make_unique<T>(std::forward<Args>(args)...);
        }

        void DetachOutput(std::string_view name)
        {
            auto it = m_outputs.find(std::string(name));
            if (it != m_outputs.end())
            {
                m_outputs.erase(it);
            }
        }

        void Write(LogLevel level, const std::string& message);
        const std::unordered_map<std::string, std::unique_ptr<ILogOutput>>& GetOutputs() const noexcept;

    private:
        std::unordered_map<std::string, std::unique_ptr<ILogOutput>> m_outputs;
    };
}  // namespace CGL::Core
