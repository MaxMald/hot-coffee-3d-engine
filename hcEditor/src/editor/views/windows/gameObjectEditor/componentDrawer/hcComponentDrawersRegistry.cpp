#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersRegistry.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcLightComponentDrawer.h"

namespace hc::editor
{
  namespace componentDrawersRegistry
  {
    void registryDefaultComponentDrawers(
      HotCoffeeEngine& hotCoffeeEngine,
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileDialogView& projectFileSelector
    )
    {
      componentDrawersManager.registerComponentView(
        MakeUnique<MeshComponentDrawer>(
          hotCoffeeEngine.getGraphicsManager().getMeshManager(),
          projectFileSelector
        )
      );
      componentDrawersManager.registerComponentView(
        MakeUnique<LightComponentDrawer>()
      );
    }
  }
}
