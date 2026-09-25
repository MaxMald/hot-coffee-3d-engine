#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor::metadata
{
  namespace fileFormat
  {
    struct EditorMetadata
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x4843454D;  // "HCEM" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hceditormeta";
    };

    struct ProjectMetadata
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x4843504D;  // "HCPM" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hcprojectmeta";
    };
  }
}
