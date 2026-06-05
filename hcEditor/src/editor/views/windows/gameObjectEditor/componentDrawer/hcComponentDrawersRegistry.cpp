#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersRegistry.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcOmniLightComponentDrawer.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcDirectionalLightComponentDrawer.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcSpotLightComponentDrawer.h"

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
        MakeUnique<OmniLightComponentDrawer>()
      );
      componentDrawersManager.registerComponentView(
        MakeUnique<DirectionalLightComponentDrawer>()
      );
      componentDrawersManager.registerComponentView(
        MakeUnique<SpotLightComponentDrawer>()
      );
    }
  }
}
