#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc::serialization
{
  /**
   * @brief File format version constants for Hot Coffee Engine.
   *
   * Defines magic numbers and version identifiers for all serialized file
   * formats. Each format has a unique magic number for validation and a
   * version number that can be incremented when the format changes.
   */
  namespace fileFormat
  {
    /**
     * @brief Scene file format (.hcscene).
     */
    struct HC_CORE_EXPORT Scene
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x48435343;  // "HCSC" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hcscene";
    };
    
    /**
     * @brief Cube map descriptor file format (.hccubemapd).
     */
    struct HC_CORE_EXPORT CubeMapDescriptor
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x48434344;  // "HCCD" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hccubemapd";
    };
    
    /**
     * @brief Prefab file format (.hcprefab).
     */
    /*
    struct Prefab
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x48435052;  // "HCPR" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hcprefab";
    };
    */
  }
}
