#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"

namespace hc
{
  namespace fileIO
  {
    /**
     * @brief Saves a string to a file at the specified path.
     *
     * @param path The path to the file where the content will be saved.
     * @param content The string content to write to the file.
     *
     * @return True if the file was written successfully, false otherwise.
     */
    HC_UTILITY_EXPORT bool SaveStringToFile(const Path& path, const String& content);

    /**
     * @brief Loads the contents of a file as a string.
     *
     * @param path The path to the file to read.
     *
     * @return An Optional containing the file contents as a string, or empty
     * Optional on failure.
     */
    HC_UTILITY_EXPORT Optional<String> LoadStringFromFile(const Path& path);
  };
}
