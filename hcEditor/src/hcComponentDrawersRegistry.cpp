#include "hc/editor/hcComponentDrawersRegistry.h"
#include "hc/editor/hcComponentDrawersManager.h"
#include "hc/editor/hcMeshComponentDrawer.h"

namespace hc::editor
{
  namespace componentDrawersRegistry
  {
    void registryDefaultComponentDrawers(
      HotCoffeeEngine& hotCoffeeEngine,
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileSelector& projectFileSelector
    )
    {
      componentDrawersManager.registerComponentView(
        MakeUnique<MeshComponentDrawer>(
          hotCoffeeEngine.getGraphicsManager().getMeshManager(),
          projectFileSelector
        )
      );
    }
  }
}
