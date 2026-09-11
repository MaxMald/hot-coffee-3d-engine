#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc::assets::metadata
{
  namespace sourceType
  {
    enum Type : UInt8
    {
      Embedded = 0,
      External = 1
    };
  }

  namespace fileFormat
  {
    struct Model
    {
      static constexpr UInt32 MAGIC_NUMBER = 0x4D444D44;  // "MDMD" in hex
      static constexpr UInt32 VERSION = 1;
      static constexpr const char* FILE_EXTENSION = ".hcmodelmeta";
    };
  }
}
