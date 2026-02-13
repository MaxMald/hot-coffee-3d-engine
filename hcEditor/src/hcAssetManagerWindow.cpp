#include "hc/editor/hcAssetManagerWindow.h"
#include "hc/editor/hcIAssetGroupDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  AssetManagerWindow::AssetManagerWindow() :
    AWindowView("Asset Manager", false)
  {
    m_allAssetExtensions.clear();
    assetFileExtensions::GetAllAssetExtensions(m_allAssetExtensions);
  }

  AssetManagerWindow::~AssetManagerWindow()
  {
  }

  void AssetManagerWindow::registerAssetGroupDrawer(
    UniquePtr<IAssetGroupDrawer> drawer
  )
  {
    if (!drawer)
    {
      throw InvalidArgumentException(
        "Cannot register a null asset group drawer."
      );
    }

    m_assetGroupDrawers.push_back(std::move(drawer));
  }

  void AssetManagerWindow::clear()
  {
    m_assetGroupDrawers.clear();
  }

  void AssetManagerWindow::onDestroy()
  {
    clear();
  }

  void AssetManagerWindow::onDraw()
  {
    for (const UniquePtr<IAssetGroupDrawer>& drawer : m_assetGroupDrawers)
      drawer->draw();
  }
}
