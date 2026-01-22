#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
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
    SharedPtr<ProjectFileSelectorView> m_fileSelectorView;
    Vector<String> m_allAssetExtensions;

    void onDraw() override;
    void drawAssetLoadingSection();
    void loadAssetFromPath(const Path& path);
  };
}
