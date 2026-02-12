#include "hc/editor/hcAssetGroupDrawersRegistry.h"
#include "hc/editor/hcAssetManagerWindow.h"
#include "hc/editor/hcImageAssetGroupDrawer.h"
#include "hc/editor/hcMaterialDescriptorAssetGroupDrawer.h"

namespace hc::editor
{
  namespace assetGroupDrawersRegistry
  {
    void registerAssetGroupDrawers(
      AssetManagerWindow& assetManagerWindow,
      AssetManager& assetManager
    )
    {
      assetManagerWindow.registerAssetGroupDrawer(
        MakeUnique<ImageAssetGroupDrawer>(assetManager)
      );
      assetManagerWindow.registerAssetGroupDrawer(
        MakeUnique<MaterialDescriptorAssetGroupDrawer>(assetManager)
      );
    }
  }
}
