#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class IAssetManagerDrawer;

  class AssetManagerWindow : public AWindowView
  {
  public:
    AssetManagerWindow();
    virtual ~AssetManagerWindow();

    void registerAssetGroupDrawer(UniquePtr<IAssetManagerDrawer> drawer);
    void clear();

  private:
    Vector<UniquePtr<IAssetManagerDrawer>> m_assetGroupDrawers;
    Vector<String> m_allAssetExtensions;

    void onDestroy() override;
    void onDraw() override;
  };
}
