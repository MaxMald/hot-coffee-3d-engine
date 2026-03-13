#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcAssetManagerDrawersRegistry.h"
#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcImageAssetManagerDrawer.h"
#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcMaterialDescriptorAssetManagerDrawer.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"

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
