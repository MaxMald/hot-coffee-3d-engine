#include "hc/editor/hcAssetManagerWindow.h"
#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include <hc/hcDependencyContainer.h>
#include <hc/hcImage.h>
#include <hc/hcMaterialDescriptor.h>
#include "hc/editor/hcEditorViewsManager.h"
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
  }

  void AssetManagerWindow::onDraw()
  {
    AssetManager& assetManager = HotCoffeeEngine::Instance().getAssetManager();

    AssetGroup<Image>& imageGroup = assetManager.getGroup<Image>();
    if (ImGui::CollapsingHeader("Images"))
    {
      for (const auto& [key, imagePtr] : imageGroup.getAll())
      {
        if (ImGui::TreeNode(key.c_str()))
        {
          ImGui::Text("Path: %s", imagePtr->getPath().c_str());
          ImGui::Text("Dimensions: %ux%u", imagePtr->getWidth(), imagePtr->getHeight());
          ImGui::TreePop();
        }
      }
    }

    AssetGroup<MaterialDescriptor>& materialGroup = assetManager.getGroup<MaterialDescriptor>();
    if (ImGui::CollapsingHeader("Materials"))
    {
      for (const auto& [key, materialPtr] : materialGroup.getAll())
      {
        if (ImGui::TreeNode(key.c_str()))
        {
          ImGui::Text("Path: %s", materialPtr->getPath().c_str());
          ImGui::TreePop();
        }
      }
    }
  }
}
