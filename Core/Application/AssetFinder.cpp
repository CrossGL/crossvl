#include "AssetFinder.h"

namespace CGL::Core
{
    std::string DataToString(const byte* data, size_t size)
    {
        return std::string(reinterpret_cast<const char*>(data), size);
    }
}  // namespace CGL::Core
