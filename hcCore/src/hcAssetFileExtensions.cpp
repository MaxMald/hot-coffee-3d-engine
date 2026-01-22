#include "hc/hcAssetFileExtensions.h"

namespace hc
{
  namespace assetFileExtensions
  {
    void HC_CORE_EXPORT GetAllAssetExtensions(Vector<String>& outExtensions)
    {
      outExtensions.push_back(MATERIAL_DESCRIPTOR);
      outExtensions.push_back(SCENE);

      for (const Char* ext : SUPPORTED_IMAGES_EXTENSIONS)
      {
        outExtensions.push_back(String(ext));
      }
    }

    bool HC_CORE_EXPORT IsMaterialDescriptorExtension(const Path& path)
    {
      return path.has_extension() && path.extension() == MATERIAL_DESCRIPTOR;
    }

    bool HC_CORE_EXPORT IsSceneExtension(const Path& path)
    {
      return path.has_extension() && path.extension() == SCENE;
    }

    bool HC_CORE_EXPORT IsImageExtension(const Path& path)
    {
      if (!path.has_extension())
        return false;

      String ext = path.extension().string();
      for (const Char* imgExt : SUPPORTED_IMAGES_EXTENSIONS)
      {
        if (ext == imgExt)
          return true;
      }

      return false;
    }
  }
}
