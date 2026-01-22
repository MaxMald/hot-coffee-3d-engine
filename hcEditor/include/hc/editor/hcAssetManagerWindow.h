#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class AssetManagerWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    AssetManagerWindow();
    virtual ~AssetManagerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  private:
    void onDraw() override;
  };
}
