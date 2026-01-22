#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class IAssetGroupDrawer;
  class ProjectFileSelectorView;

  class AssetManagerWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    AssetManagerWindow();
    virtual ~AssetManagerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  private:
    Vector<SharedPtr<IAssetGroupDrawer>> m_assetGroupDrawers;
    SharedPtr<ProjectFileSelectorView> m_fileSelectorView;
    Vector<String> m_allAssetExtensions;

    void onDraw() override;
    void registerAssetGroupDrawers();
    void drawAssetLoadingSection();
    void loadAssetFromPath(const Path& path);
  };
}
