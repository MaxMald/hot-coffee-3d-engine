#include "hc/editor/hcAssetManagerWindow.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include <hc/hcDependencyContainer.h>
#include <hc/hcImage.h>
#include <hc/hcMaterialDescriptor.h>
#include <hc/hcAssetFileExtensions.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelectorView.h"
#include "hc/editor/hcIAssetGroupDrawer.h"
#include "hc/editor/hcImageAssetGroupDrawer.h"
#include "hc/editor/hcMaterialDescriptorAssetGroupDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  AssetManagerWindow::AssetManagerWindow() :
    AWindowView("Asset Manager", false)
  {
  }

  AssetManagerWindow::~AssetManagerWindow()
  {
  }

  void AssetManagerWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_fileSelectorView = container.resolve<ProjectFileSelectorView>();

    m_allAssetExtensions.clear();
    assetFileExtensions::GetAllAssetExtensions(m_allAssetExtensions);

    registerAssetGroupDrawers();
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
