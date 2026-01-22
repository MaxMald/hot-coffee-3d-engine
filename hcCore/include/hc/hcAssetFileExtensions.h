#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace assetFileExtensions
  {
    constexpr const char* MATERIAL_DESCRIPTOR = ".hcmatdesc";
    constexpr const char* SCENE = ".hcscene";

    inline const Array<const char*, 4> SUPPORTED_IMAGES_EXTENSIONS =
    {
      ".png",
      ".jpg",
      ".jpeg",
      ".tga"
    };
  }
}
