#include "hc/utilities/hcPath.h"

namespace hc
{
  Path::Path() : m_path(), m_type(pathType::Undefined) {}
  Path::Path(const std::filesystem::path& path, pathType::Type type) : m_path(path), m_type(type) {}
  Path::Path(const String& path, pathType::Type type) : m_path(path.c_str()), m_type(type) {}
  Path::Path(const char* path, pathType::Type type) : m_path(path), m_type(type) {}

  Path::operator std::filesystem::path() const
  {
    return m_path;
  }

  Path::operator String() const
  {
    return String(m_path.string());
  }

  Path Path::operator/ (const Path& other) const
  {
    return Path(m_path / other.m_path, m_type);
  }

  Path& Path::operator/= (const Path& other)
  {
    m_path /= other.m_path;
    return *this;
  }

  Path Path::operator/(const String& other) const
  {
    return Path(m_path / other.c_str(), m_type);
  }

  Path& Path::operator/=(const String& other)
  {
    m_path /= other.c_str();
    return *this;
  }

  Path Path::operator/(const Char* other) const
  {
    return Path(m_path / other, m_type);
  }

  Path& Path::operator/=(const Char* other)
  {
    m_path /= other;
    return *this;
  }

  bool Path::operator==(const Path& other) const
  {
    return m_path == other.m_path;
  }

  bool Path::operator!=(const Path& other) const
  {
    return !(*this == other);
  }

  bool Path::operator==(const String& other) const
  { 
    return m_path == other.c_str();
  }

  bool Path::operator!=(const String& other) const
  {
    return !(*this == other);
  }

  bool Path::operator==(const Char* other) const
  {
    return m_path == other;
  }

  bool Path::operator!=(const Char* other) const
  {
    return !(*this == other);
  }

  const std::filesystem::path& Path::getPath() const
  {
    return m_path;
  }

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

  Path operator/ (const String& left, const Path& right)
  {
    return Path(left) / right;
  }

  Path operator/ (const Char* left, const Path& right)
  {
    return Path(left) / right;
  }
}
