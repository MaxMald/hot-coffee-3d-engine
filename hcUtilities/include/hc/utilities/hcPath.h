#pragma once

#include <filesystem>

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"

namespace hc
{
  namespace pathType
  {
    enum Type : UInt8
    {
      // Full filesystem paths from the root
      Absolute = 0,

      // Paths relative to a specific root directory
      Relative = 1,

      // Paths to resources embedded in files or packages
      Embedded = 2,

      // Internal engine resources
      Internal = 3,

      // Just the filename component (with or without extension)
      Filename = 4,

      // File extension only
      Extension = 5
    };
  }

  /**
   * @brief Manages file system paths with support for absolute, relative, embedded, and
   * internal path types.
   *
   * The Path class wraps std::filesystem::path and adds semantic path type information.
   * It supports the following path types:
   * - **Absolute**: Full filesystem paths from the root
   * - **Relative**: Paths relative to a specific root directory
   * - **Embedded**: Paths to resources embedded in files or packages
   * - **Internal**: Internal engine resources
   * - **Filename**: Just the filename component (with or without extension)
   * - **Extension**: File extension only
   *
   * Path is hashable and can be used as a key in unordered containers.
   *
   * @note Equality comparison is based only on the path string value, not the path type.
   * @note Hash computation is based only on the path string value, not the path type.
   */
  class HC_UTILITY_EXPORT Path
  {
  public:
    /**
     * @brief Creates an empty absolute path.
     */
    Path();

    /**
     * @brief Creates a Path from a std::filesystem::path with the specified type.
     * @param path The filesystem path
     * @param type The path type (default: Absolute)
     */
    Path(const std::filesystem::path& path, pathType::Type type = pathType::Absolute);

    /**
     * @brief Creates a Path from a String with the specified type.
     * @param path The path as a String
     * @param type The path type (default: Absolute)
     */
    Path(const String& path, pathType::Type type = pathType::Absolute);

    /**
     * @brief Creates a Path from a C-string with the specified type.
     * @param path The path as a C-string
     * @param type The path type (default: Absolute)
     */
    Path(const char* path, pathType::Type type = pathType::Absolute);

    /**
     * @brief Converts the Path to a std::filesystem::path.
     */
    operator std::filesystem::path();

    /**
     * @brief Converts the Path to a String representation.
     */
    operator String() const;

    /**
     * @brief Appends a path component to this path (non-modifying).
     * @param other The path to append (must not be absolute)
     * @return A new Path with the appended component
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path operator/ (const Path& other) const;

    /**
     * @brief Appends a path component to this path (in-place).
     * @param other The path to append (must not be absolute)
     * @return Reference to this Path
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path& operator/= (const Path& other);

    /**
     * @brief Appends a String path component to this path (non-modifying).
     * @param other The path component as a String
     * @return A new Path with the appended component
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path operator/ (const String& other) const;

    /**
     * @brief Appends a String path component to this path (in-place).
     * @param other The path component as a String
     * @return Reference to this Path
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path& operator/= (const String& other);

    /**
     * @brief Appends a C-string path component to this path (non-modifying).
     * @param other The path component as a C-string
     * @return A new Path with the appended component
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path operator/ (const Char* other) const;

    /**
     * @brief Appends a C-string path component to this path (in-place).
     * @param other The path component as a C-string
     * @return Reference to this Path
     * @throws InvalidArgumentException if other is an absolute path
     */
    Path& operator/= (const Char* other);

    /**
     * @brief Checks equality with another Path.
     * @param other The Path to compare with
     * @return true if the path string values are equal (type is not compared), false otherwise
     * @note This compares only the path string content, not the path type.
     */
    bool operator== (const Path& other) const;

    /**
     * @brief Checks inequality with another Path.
     * @param other The Path to compare with
     * @return true if the path string values differ, false otherwise
     * @note This compares only the path string content, not the path type.
     */
    bool operator!= (const Path& other) const;

    /**
     * @brief Checks equality with a String path.
     * @param other The String to compare with
     * @return true if the path matches the string, false otherwise
     */
    bool operator== (const String& other) const;

    /**
     * @brief Checks inequality with a String path.
     * @param other The String to compare with
     * @return true if the path does not match the string, false otherwise
     */
    bool operator!= (const String& other) const;

    /**
     * @brief Checks equality with a C-string path.
     * @param other The C-string to compare with
     * @return true if the path matches the string, false otherwise
     */
    bool operator== (const Char* other) const;

    /**
     * @brief Checks inequality with a C-string path.
     * @param other The C-string to compare with
     * @return true if the path does not match the string, false otherwise
     */
    bool operator!= (const Char* other) const;

    /**
     * @brief Retrieves the underlying std::filesystem::path.
     * @return Const reference to the internal filesystem path
     */
    const std::filesystem::path& getPath() const;

    /**
     * @brief Sets the path from a String and type (overload 1).
     * @param path The path as a String
     * @param type The path type (default: Absolute)
     */
    inline void set(const String& path, pathType::Type type = pathType::Absolute)
    {
      m_path = std::filesystem::path(path.c_str());
      m_type = type;
    }

    /**
     * @brief Sets the path from a C-string and type (overload 2).
     * @param path The path as a C-string
     * @param type The path type (default: Absolute)
     */
    inline void set(const char* path, pathType::Type type = pathType::Absolute)
    {
      m_path = std::filesystem::path(path);
      m_type = type;
    }

    /**
     * @brief Sets the path from std::filesystem::path and type (overload 3).
     * @param path The filesystem path
     * @param type The path type (default: Absolute)
     */
    inline void set(const std::filesystem::path& path, pathType::Type type = pathType::Absolute)
    {
      m_path = path;
      m_type = type;
    }

    /**
     * @brief Converts this path to a relative path with respect to a root directory.
     * @param rootPath The root directory to make this path relative to
     * @return A new Path with type Relative
     * @throws InvalidArgumentException if this path is not under the root directory
     */
    Path toRelative(const Path& rootPath) const;

    /**
     * @brief Converts this relative path to an absolute path using a root directory.
     * @param rootPath The root directory to resolve relative paths against
     * @return A new Path with absolute type
     * @throws InvalidArgumentException if this path is not relative or escapes the root
     */
    Path toAbsolute(const Path& rootPath) const;

    /**
     * @brief Creates a copy of this path with Embedded type.
     * @return A new Path with the same filesystem path but Embedded type
     */
    inline Path toEmbedded() const noexcept
    {
      return Path(m_path, pathType::Embedded);
    }

    /**
     * @brief Checks if this path is relative.
     * @return true if the path type is Relative, false otherwise
     */
    inline bool isRelative() const noexcept
    {
      return m_type == pathType::Relative;
    }

    /**
     * @brief Checks if this path is absolute.
     * @return true if the path type is Absolute, false otherwise
     */
    inline bool isAbsolute() const noexcept
    {
      return m_type == pathType::Absolute;
    }

    /**
     * @brief Checks if this path is embedded.
     * @return true if the path type is Embedded, false otherwise
     */
    inline bool isEmbedded() const noexcept
    {
      return m_type == pathType::Embedded;
    }

    inline const std::filesystem::path::value_type* c_str() const noexcept
    {
      return m_path.c_str();
    }

    /**
     * @brief Checks if this path is empty.
     * @return true if the path is empty, false otherwise
     */
    inline bool empty() const noexcept
    {
      return m_path.empty();
    }

    /**
     * @brief Clears the path and resets type to Absolute.
     */
    inline void clear() noexcept
    {
      m_path.clear();
      m_type = pathType::Absolute;
    }

    /**
     * @brief Checks if this path has a file extension.
     * @return true if the path has an extension, false otherwise
     */
    inline bool hasExtension() const noexcept
    {
      return m_path.has_extension();
    }

    /**
     * @brief Gets the file extension component of this path.
     * @return A new Path representing the extension with type Extension
     */
    inline Path extension() const
    {
      return Path(m_path.extension(), pathType::Extension);
    }

    /**
     * @brief Gets the stem (filename without extension) of this path.
     * @return A new Path representing the stem with type Filename
     */
    inline Path stem() const
    {
      return Path(m_path.stem(), pathType::Filename);
    }

    /**
     * @brief Gets the filename component of this path.
     * @return A new Path representing the filename with type Filename
     */
    inline Path filename() const
    {
      return Path(m_path.filename(), pathType::Filename);
    }

    /**
     * @brief Gets the parent directory of this path.
     * @return A new Path representing the parent directory with the same type as this path
     */
    inline Path parentPath() const
    {
      return Path(m_path.parent_path(), m_type);
    }

    /**
     * @brief Gets the path type of this Path.
     * @return The pathType::Type value
     */
    inline pathType::Type getType() const noexcept
    {
      return m_type;
    }

    /**
     * @brief Checks if this path is under a given root directory.
     * @param rootPath The root directory to check against
     * @return true if this path is under the root, false otherwise
     * @throws InvalidArgumentException if this path is not absolute
     */
    bool isUnderRoot(const Path& rootPath) const;

    /**
     * @brief Converts this path to a generic string representation using forward slashes.
     * @return The path as a String with forward slash separators
     */
    inline String toGenericString() const
    {
      return String(m_path.generic_string());
    }

    /**
     * @brief Converts this path to a native string representation.
     * @return The path as a String with native path separators
     */
    inline String toString() const
    {
      return String(m_path.string());
    }

  private:
    std::filesystem::path m_path;
    pathType::Type m_type;
  };

  /**
   * @brief Appends a Path to a String (free function overload).
   * @param left The left-side String
   * @param right The right-side Path to append
   * @return A new Path with the appended component
   * @throws InvalidArgumentException if right is an absolute path
   */
  HC_UTILITY_EXPORT Path operator/ (const String& left, const Path& right);

  /**
   * @brief Appends a Path to a C-string (free function overload).
   * @param left The left-side C-string
   * @param right The right-side Path to append
   * @return A new Path with the appended component
   * @throws InvalidArgumentException if right is an absolute path
   */
  HC_UTILITY_EXPORT Path operator/ (const Char* left, const Path& right);
}

namespace std
{
  /**
   * @brief Hash specialization for hc::Path.
   *
   * Computes a hash based only on the path string value, not the path type.
   * This allows Path objects to be used as keys in unordered containers like
   * std::unordered_map and std::unordered_set.
   *
   * @note The hash is invariant to path type; two paths with the same string
   *       but different types will produce the same hash value.
   */
  template <>
  struct hash<hc::Path>
  {
    /**
     * @brief Computes the hash value for a Path.
     * @param path The Path to hash
     * @return The hash value based on the path's generic string representation
     */
    size_t operator()(const hc::Path& path) const noexcept
    {
      return std::hash<std::string>()(path.toGenericString());
    }
  };
}
