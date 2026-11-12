#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersRegistry.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcLightComponentDrawer.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"
#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

namespace hc::editor
{
  namespace componentDrawersRegistry
  {
    void registryDefaultComponentDrawers(
      HotCoffeeEngine& hotCoffeeEngine,
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileDialogView& projectFileSelector,
      EditorServiceManager& editorServiceManager
    )
    {
      componentDrawersManager.registerComponentView(
        MakeUnique<MeshComponentDrawer>(
          hotCoffeeEngine.getGraphicsManager().getMeshManager(),
          hotCoffeeEngine.getAssetManager(),
          projectFileSelector,
          editorServiceManager.getService<MaterialDrawersManager>(),
          editorServiceManager.getService<EditorMetadataManager>()
        )
      );
      componentDrawersManager.registerComponentView(
        MakeUnique<LightComponentDrawer>()
      );
    }
  }
}
