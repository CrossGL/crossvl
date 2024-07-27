#pragma once
#include <string>

namespace CGL::Core
{
	std::string GetAssetPath(std::string_view rootPath, std::string_view assetName);
	std::wstring GetAssetPathW(std::string_view rootPath, std::string_view assetName);
}
