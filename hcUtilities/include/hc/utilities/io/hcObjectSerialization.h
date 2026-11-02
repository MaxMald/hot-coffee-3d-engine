#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"

namespace hc::io
{
  /**
   * @brief Represents the header of a serialized object.
   *
   * This information is used during serialization and deserialization to ensure
   * compatibility and proper handling of the object's data.
   */
  struct alignas(8) ObjectHeader
  {
    UInt64 size = static_cast<UInt64>(0);    ///< The size of the object in bytes (including the header).
    UInt32 type = static_cast<UInt32>(0);    ///< The type identifier of the object.
    UInt32 version = static_cast<UInt32>(0); ///< The version of the object.

    /**
     * @brief Checks if the header matches a given version.
     *
     * @param version The version to compare against.
     * 
     * @return True if the versions match, false otherwise.
     */
    bool matchVersion(UInt32 _version) const
    {
      return this->version == _version;
    }

    /**
     * @brief Checks if the header matches a given type.
     *
     * @param type The type to compare against.
     *
     * @return True if the types match, false otherwise.
     */
    bool matchType(UInt32 _type) const
    {
      return this->type == _type;
    }

    /**
     * @brief Checks if the header matches a given type and version.
     *
     * @param type The type to compare against.
     * @param version The version to compare against.
     * 
     * @return True if both the type and version match, false otherwise.
     */
    bool match(const UInt32& _type, UInt32 _version) const
    {
      return this->type == _type && this->version == _version;
    }
  };

  /**
   * @brief Represents tracking information for an object being read or written in a
   * binary stream.
   */
  struct ObjectTrackingInfo
  {
    std::streampos startPosition = static_cast<std::streampos>(-1); ///< The starting position of the object in the stream.
    std::streampos endPosition = static_cast<std::streampos>(-1);   ///< The ending position of the object in the stream.
  };
}
