#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace assetFileExtensions
  {
    constexpr const Char* MATERIAL_DESCRIPTOR = ".hcmatdesc";
    constexpr const Char* SCENE = ".hcscene";

    inline const Array<const Char*, 4> SUPPORTED_IMAGES_EXTENSIONS =
    {
      ".png",
      ".jpg",
      ".jpeg",
      ".tga"
    };

    void HC_CORE_EXPORT GetAllAssetExtensions(Vector<String>& outExtensions);

    bool HC_CORE_EXPORT IsMaterialDescriptorExtension(const Path& path);
    bool HC_CORE_EXPORT IsSceneExtension(const Path& path);
    bool HC_CORE_EXPORT IsImageExtension(const Path& path);
  }
}
