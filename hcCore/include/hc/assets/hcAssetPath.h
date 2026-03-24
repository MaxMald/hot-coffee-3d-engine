#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Utility class for converting between absolute and relative asset
   * paths.
   *
   * AssetPath provides methods to convert asset paths between absolute and
   * relative formats.
   */
  struct HC_CORE_EXPORT AssetPath
  {
    /**
     * @brief Converts absolute path to project-relative path.
     *
     * @param absolutePath Absolute filesystem path.
     * @param projectRoot Root directory of the project.
     * 
     * @return Relative path from project root.
     */
    static String ToRelative(const Path& absolutePath, const Path& rootPath);

    /**
     * @brief Converts project-relative path to absolute path.
     *
     * @param relativePath Path relative to project root.
     * @param projectRoot Root directory of the project.
     * 
     * @return Absolute filesystem path.
     */
    static Path ToAbsolute(const String& relativePath, const Path& rootPath);

    /**
     * @brief Checks if a path is relative.
     *
     * @param path The path to check.
     * 
     * @return True if the path is relative, false if it's absolute.
     */
    static bool IsRelative(const Path& path);

    /**
     * @brief Checks if a string path is relative.
     *
     * @param path The string path to check.
     *
     * @return True if the string path is relative, false if it's absolute.
     */
    static bool IsRelative(const String& path);

    /**
     * @brief Checks if an absolute path is under the specified root path.
     *
     * @param absolutePath The absolute path to check.
     * @param rootPath The root path to check against.
     *
     * @return True if the absolute path is under the root path, false otherwise.
     */
    static bool IsUnderRoot(const Path& absolutePath, const Path& rootPath);
  };
}
