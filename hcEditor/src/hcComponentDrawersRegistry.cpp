#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersRegistry.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"

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
