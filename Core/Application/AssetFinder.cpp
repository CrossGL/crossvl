#include "AssetFinder.h"
#include <filesystem>

namespace CGL::Core
{
	std::string GetAssetPath(std::string_view rootPath, std::string_view assetName)
	{
		std::filesystem::path path(rootPath);
		path.append(assetName);

		return std::filesystem::exists(path) ? path.string() : "";
	}
		
	std::wstring GetAssetPathW(std::string_view rootPath, std::string_view assetName)
	{
		std::filesystem::path path(rootPath);
		path.append(assetName);

		return std::filesystem::exists(path) ? path.wstring() : L"";
	}
}
