#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/hcEditorCameraController.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

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
      serviceManager.registerService<ProjectManager>(
        MakeUnique<ProjectManager>(engine.getAssetManager())
      );
      serviceManager.registerService<GameObjectSelectionService>(
        MakeUnique<GameObjectSelectionService>()
      );
      serviceManager.registerService<EditorCameraController>(
        MakeUnique<EditorCameraController>(
          engine.getSceneManager(),
          engine.getInputManager()
        )
      );
      serviceManager.registerService<EditorSceneManager>(
        MakeUnique<EditorSceneManager>(
          editorScene,
          serviceManager.getService<ProjectManager>()
        )
      );
    }
  }
}
