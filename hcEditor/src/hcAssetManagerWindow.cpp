#include "hc/editor/hcAssetManagerWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelector.h"
#include "hc/editor/hcIAssetGroupDrawer.h"
#include "hc/editor/hcImageAssetGroupDrawer.h"
#include "hc/editor/hcMaterialDescriptorAssetGroupDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  AssetManagerWindow::AssetManagerWindow() :
    AWindowView("Asset Manager", false)
  {
    m_allAssetExtensions.clear();
    assetFileExtensions::GetAllAssetExtensions(m_allAssetExtensions);

    registerAssetGroupDrawers();
  }

  AssetManagerWindow::~AssetManagerWindow()
  {
  }

  void AssetManagerWindow::onDraw()
  {
    for (const SharedPtr<IAssetGroupDrawer>& drawer : m_assetGroupDrawers)
      drawer->draw();
  }

  void AssetManagerWindow::registerAssetGroupDrawers()
  {
    m_assetGroupDrawers.push_back(MakeShared<ImageAssetGroupDrawer>());
    m_assetGroupDrawers.push_back(MakeShared<MaterialDescriptorAssetGroupDrawer>());
  }
}
