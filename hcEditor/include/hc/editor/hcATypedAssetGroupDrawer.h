#pragma once

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include "hc/editor/hcIAssetGroupDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  template<typename T>
  class ATypedAssetGroupDrawer : public IAssetGroupDrawer
  {
  public:
    ATypedAssetGroupDrawer() = default;
    virtual ~ATypedAssetGroupDrawer() = default;

    void draw() override;

  protected:
    virtual void drawAssetDetails(const SharedPtr<T>& asset) = 0;
  };

  template<typename T>
  void ATypedAssetGroupDrawer<T>::draw()
  {
    AssetManager& assetManager = HotCoffeeEngine::Instance().getAssetManager();
    AssetGroup<T>& assetGroup = assetManager.getGroup<T>();

    String headerTitle = String::Format(
      "%s Assets (%zu)",
      typeid(T).name(),
      assetGroup.size()
    );

    if (ImGui::CollapsingHeader(headerTitle.c_str()))
    {
      for (const auto& [key, assetPtr] : assetGroup.getAll())
      {
        if (ImGui::TreeNode(key.c_str()))
        {
          Path path = assetPtr->getPath();
          ImGui::Text("Path: %s", path.c_str());
          drawAssetDetails(assetPtr);
          ImGui::TreePop();
        }
      }
    }
  }
}
