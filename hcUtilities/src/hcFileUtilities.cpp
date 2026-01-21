#include "hc/hcFileUtilities.h"
#include <fstream>

namespace hc
{
  namespace fileUtilities
  {
    bool SaveStringToFile(const Path& path, const String& content)
    {
      std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
      if (!file.is_open())
        return false;

      file << content;
      file.close();
      return true;
    }

    Optional<String> LoadStringFromFile(const Path& path)
    {
      std::ifstream file(path.c_str(), std::ios::in);
      if (!file.is_open())
        return {};

      String content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
      );

      file.close();
      return content;
    }
  }
}
