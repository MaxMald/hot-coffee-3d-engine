#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"
#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

namespace hc::editor
{
  namespace editorServiceManagerRegistry
  {
    void registerServices(
      HotCoffeeEngine& engine,
      EditorServiceManager& serviceManager,
      Scene* editorScene
    )
    {
      // Services are registered in the order of their dependencies. Services that depend
      // on other services should be registered after their dependencies.

      UniquePtr<EditorMetadataManager> editorMetadataManager = MakeUnique<EditorMetadataManager>(
        engine.getAssetManager()
      );

      serviceManager.registerService<ProjectManager>(
        MakeUnique<ProjectManager>(engine.getAssetManager(), *editorMetadataManager)
      );
      serviceManager.registerService<GameObjectSelectionService>(
        MakeUnique<GameObjectSelectionService>()
      );
      serviceManager.registerService<EditorSceneManager>(
        MakeUnique<EditorSceneManager>(
          editorScene,
          engine.getAssetManager(),
          engine.getGraphicsManager(),
          serviceManager.getService<ProjectManager>(),
          *editorMetadataManager
        )
      );
      serviceManager.registerService<MaterialDrawersManager>(
        MakeUnique<MaterialDrawersManager>(engine.getGraphicsManager().getTextureManager())
      );

      // Register services that do not have dependencies on other services after all
      // dependent services have been registered.

      serviceManager.registerService<EditorMetadataManager>(std::move(editorMetadataManager));
    }
  }
}
