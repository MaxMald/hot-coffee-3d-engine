#include "hc/editor/hcComponentDrawersRegistry.h"
#include "hc/editor/hcComponentDrawersManager.h"
#include "hc/editor/hcMeshComponentDrawer.h"

namespace hc::editor
{
  namespace componentDrawersRegistry
  {
    void registryDefaultComponentDrawers(
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileSelector& projectFileSelector
    )
    {
      componentDrawersManager.registerComponentView(
        MakeUnique<MeshComponentDrawer>(projectFileSelector)
      );
    }
  }
}
