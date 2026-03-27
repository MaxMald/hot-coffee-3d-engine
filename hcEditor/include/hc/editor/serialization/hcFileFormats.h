#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor::serialization
{
  namespace fileFormats
  {
    struct Project
    {
      static constexpr UInt32 MAGIC = 0x48435052;  // "HCPR" in hex
      static constexpr UInt16 MAJOR_VERSION = 1;
      static constexpr UInt16 MINOR_VERSION = 0;
      static constexpr UInt16 PATCH_VERSION = 0;
      static constexpr const char* FILE_EXTENSION = ".hcproj";
    };
  }
}
