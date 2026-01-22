#include "hc/editor/hcAssetDependenciesLoader.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include <hc/hcMaterialDescriptor.h>
#include <hc/hcImage.h>

namespace hc::editor
{
  namespace assetDependenciesLoader
  {
    void loadDependenciesForMaterialDescriptor(
      const Path& assetPath,
      SharedPtr<MaterialDescriptor>& materialDescriptor
    )
    {
      AssetManager& assetManager = HotCoffeeEngine::Instance().getAssetManager();
      Vector<String> requiredImages = materialDescriptor->getImagesKeys();

      for (const String& imageKey : requiredImages)
      {
        if (!assetManager.contains<Image>(imageKey))
          assetManager.load<Image>(imageKey, Path(imageKey.c_str()));
      }
    }
  }
}
