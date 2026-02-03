#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class IAssetGroupDrawer;

  class AssetManagerWindow : public AWindowView
  {
  public:
    AssetManagerWindow();
    virtual ~AssetManagerWindow();

  private:
    Vector<SharedPtr<IAssetGroupDrawer>> m_assetGroupDrawers;
    Vector<String> m_allAssetExtensions;

    void onDraw() override;
    void registerAssetGroupDrawers();
  };
}
