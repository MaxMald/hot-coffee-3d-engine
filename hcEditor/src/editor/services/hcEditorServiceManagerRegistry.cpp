#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/hcEditorCameraController.h"

namespace hc::editor
{
  namespace editorServiceManagerRegistry
  {
    void registerServices(
      HotCoffeeEngine& engine,
      EditorServiceManager& serviceManager
    )
    {
      serviceManager.registerService<ProjectManager>(
        MakeUnique<ProjectManager>()
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
    }
  }
}
