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

    void registerAssetGroupDrawer(UniquePtr<IAssetGroupDrawer> drawer);
    void clear();

  private:
    Vector<UniquePtr<IAssetGroupDrawer>> m_assetGroupDrawers;
    Vector<String> m_allAssetExtensions;

    void onDestroy() override;
    void onDraw() override;
  };
}
