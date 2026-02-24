#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcAssetGroupDrawersRegistry.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcImageAssetGroupDrawer.h"
#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcMaterialDescriptorAssetGroupDrawer.h"

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
