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

      UniquePtr<ProjectManager> projectManager = MakeUnique<ProjectManager>(
        engine.getAssetManager(),
        *editorMetadataManager
      );

      UniquePtr<EditorSceneManager> editorSceneManager = MakeUnique<EditorSceneManager>(
        editorScene,
        engine.getAssetManager(),
        engine.getGraphicsManager(),
        *projectManager,
        *editorMetadataManager
      );

      serviceManager.registerService<GameObjectSelectionService>(
        MakeUnique<GameObjectSelectionService>(*editorSceneManager)
      );
      serviceManager.registerService<MaterialDrawersManager>(
        MakeUnique<MaterialDrawersManager>(engine.getGraphicsManager().getTextureManager())
      );

      // Register services that do not have dependencies on other services after all
      // dependent services have been registered.

      serviceManager.registerService<EditorSceneManager>(std::move(editorSceneManager));
      serviceManager.registerService<ProjectManager>(std::move(projectManager));
      serviceManager.registerService<EditorMetadataManager>(std::move(editorMetadataManager));
    }
  }
}
