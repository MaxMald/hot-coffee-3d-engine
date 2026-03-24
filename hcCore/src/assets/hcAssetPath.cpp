#include "hc/assets/hcAssetPath.h"
#include <filesystem>

namespace hc
{
  static constexpr char ASSET_PATH_PREFIX = '@';

  String AssetPath::ToRelative(const Path& absolutePath, const Path& rootPath)
  {
    Path relative = std::filesystem::relative(absolutePath, rootPath);
    return ASSET_PATH_PREFIX + relative.generic_string();
  }

  Path AssetPath::ToAbsolute(const String& relativePath, const Path& rootPath)
  {
    if (!IsRelative(relativePath)) {
      return Path(relativePath.c_str());
    }

    String relativePart = relativePath.substr(1);
    return rootPath / Path(relativePart.c_str());
  }

  bool AssetPath::IsRelative(const Path& path)
  {
    return !path.empty() && path.string()[0] == ASSET_PATH_PREFIX;
  }

  bool AssetPath::IsRelative(const String& path)
  {
    return !path.empty() && path[0] == ASSET_PATH_PREFIX;
  }
}
