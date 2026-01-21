#include "hc/hcFileIO.h"
#include <fstream>

namespace hc
{
  namespace fileIO
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
