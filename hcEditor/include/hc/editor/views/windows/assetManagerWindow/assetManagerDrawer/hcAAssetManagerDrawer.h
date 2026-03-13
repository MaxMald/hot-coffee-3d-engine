#pragma once

#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcIAssetManagerDrawer.h"
#include <imgui.h>

namespace hc::editor
{
  /**
   * Abstract base class for asset manager drawers.
   *
   * Provides an interface for drawing asset management UI components for
   * specific asset types.
   *
   * @tparam AssetType The type of asset this drawer manages
   */
  template<typename AssetType>
  class AAssetManagerDrawer : public IAssetManagerDrawer
  {
  public:
    virtual ~AAssetManagerDrawer() = default;

    /**
     * Draws the asset manager UI.
     */
    void draw() override;

  protected:
    /**
     * Constructs an asset manager drawer.
     *
     * @param typedAssetManager Reference to the typed asset manager
     */
    AAssetManagerDrawer(ATypedAssetManager<AssetType>& typedAssetManager);

    /**
     * Draws detailed information for a specific asset.
     *
     * @param asset The asset to display details for
     */
    virtual void drawAssetDetails(const SharedPtr<AssetType>& asset) = 0;

  private:
    ATypedAssetManager<AssetType>& m_typedAssetManager;
    Vector<SharedPtr<AssetType>> m_loadedAssets;
  };

  template<typename AssetType>
  void AAssetManagerDrawer<AssetType>::draw()
  {
    String headerTitle = String::Format(
      "%s Assets (%zu)",
      typeid(AssetType).name(),
      m_typedAssetManager.size()
    );

    if (ImGui::CollapsingHeader(headerTitle.c_str()))
    {
      m_typedAssetManager.getAllLoadedAssets(m_loadedAssets);

      for (const auto& assetPtr : m_loadedAssets)
      {
        String pathStr = assetPtr->getPath().string();
        if (ImGui::TreeNode(pathStr.c_str()))
        {
          ImGui::Text("Path: %s", pathStr.c_str());
          drawAssetDetails(assetPtr);
          ImGui::TreePop();
        }
      }
    }
  }

  template<typename AssetType>
  AAssetManagerDrawer<AssetType>::AAssetManagerDrawer(
    ATypedAssetManager<AssetType>& typedAssetManager
  ) :
    m_typedAssetManager(typedAssetManager),
    m_loadedAssets()
  {
  }
}
