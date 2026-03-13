#pragma once

#include <string>
#include <cstdarg>
#include <vector>

/**
 * @brief A string class that extends std::string with additional utilities.
 */
class String : public std::string
{
public:
  using std::string::string;

  String(const std::string& s) : std::string(s) {}

  /**
   * @brief Formats a string using printf-style formatting.
   *
   * Creates a formatted string using the specified format and arguments. This
   * method behaves similarly to C#'s String.Format or C's sprintf.
   *
   * @param fmt The format string (printf-style).
   * @param ... Variable arguments to format.
   * @return A formatted String object.
   *
   * @note
   *   - The format string must be compatible with printf.
   *   - Buffer resizing is handled automatically for long output.
   *   - Returns an empty String if formatting fails.
   */
  static String Format(const char* fmt, ...)
  {
    va_list args;

    va_start(args, fmt);

    std::vector<char> buffer(1024);
    int len = std::vsnprintf(buffer.data(), buffer.size(), fmt, args);

    va_end(args);

    if (len < 0) {
      return String();
    }

    if (static_cast<size_t>(len) >= buffer.size()) {
      buffer.resize(len + 1);

      va_start(args, fmt);
      std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
      va_end(args);
    }

    return String(buffer.data());
  }
};

namespace std {
  /**
   * @brief Specialization of std::hash for the String class.
   *
   * Enables the use of String as a key in unordered associative containers such
   * as std::unordered_map and std::unordered_set by providing a hash function.
   * This implementation delegates hashing to std::string's hash function,
   * ensuring consistent behavior and performance.
   */
  template<>
  struct hash<String>
  {
    /**
     * @brief Computes the hash value for a String object.
     *
     * @param s The String object to hash.
     * @return The hash value as a std::size_t.
     */
    std::size_t operator()(const String& s) const noexcept
    {
      return std::hash<std::string>()(static_cast<const std::string&>(s));
    }
  };
}
