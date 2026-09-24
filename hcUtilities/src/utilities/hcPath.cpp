#include "hc/utilities/hcPath.h"

namespace hc
{
  Path::Path() : m_path(){}
  Path::Path(const std::filesystem::path& path) : m_path(path) {}
  Path::Path(const String& path) : m_path(path.c_str()) {}
  Path::Path(const char* path) : m_path(path) {}

  Path Path::toRelative(const Path& rootPath) const
  {
    if (!isUnderRoot(rootPath))
    {
      throw InvalidArgumentException(
        String::Format(
          "Cannot set path as relative: path is not under root directory. Path: %s, Root path: %s",
          toGenericString().c_str(),
          rootPath.toGenericString().c_str()
        )
      );
    }

    std::filesystem::path resultPath = std::filesystem::relative(
      getPath(),
      rootPath.getPath()
    );

    return Path(resultPath);
  }

  Path Path::toAbsolute(const Path& rootPath) const
  {
    if (rootPath.isRelative())
    {
      throw InvalidArgumentException(
        String::Format(
          "Cannot set path as absolute: root path is not absolute. Root path: %s",
          rootPath.toGenericString().c_str()
        )
      );
    }

    if (!isRelative())
    {
      throw InvalidArgumentException(
        String::Format(
          "Cannot set path as absolute: path is not relative. Path: %s",
          toGenericString().c_str()
        )
      );
    }

    Path resultPath(rootPath.getPath() / m_path);
    if (!resultPath.isUnderRoot(rootPath))
    {
      throw InvalidArgumentException(
        String::Format(
          "Cannot set path as absolute: resolved path escapes root directory. Resolved path: %s, Root path: %s",
          resultPath.toGenericString().c_str(),
          rootPath.toGenericString().c_str()
        )
      );
    }

    return resultPath;
  }

  bool Path::isUnderRoot(const Path& rootPath) const
  {
    std::filesystem::path canonicalAbsolute = std::filesystem::weakly_canonical(m_path);
    std::filesystem::path canonicalRoot = std::filesystem::weakly_canonical(rootPath.m_path);

    auto [rootEnd, absEnd] = std::mismatch(
      canonicalRoot.begin(), canonicalRoot.end(),
      canonicalAbsolute.begin(), canonicalAbsolute.end()
    );

    return rootEnd == canonicalRoot.end();
  }
}
