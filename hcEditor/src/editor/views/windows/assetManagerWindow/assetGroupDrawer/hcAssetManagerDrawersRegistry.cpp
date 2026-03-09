#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcAssetManagerDrawersRegistry.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcImageAssetManagerDrawer.h"
#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcMaterialDescriptorAssetManagerDrawer.h"

namespace hc::editor
{
  namespace assetManagerDrawersRegistry
  {
    void registerAssetGroupDrawers(
      AssetManagerWindow& assetManagerWindow,
      IAssetManager& assetManager
    )
    {
      assetManagerWindow.registerAssetGroupDrawer(
        MakeUnique<ImageAssetManagerDrawer>(assetManager.getImageAssetManager())
      );
      assetManagerWindow.registerAssetGroupDrawer(
        MakeUnique<MaterialDescriptorAssetManagerDrawer>(assetManager.getMaterialDescriptorAssetManager())
      );
    }
  }
}
