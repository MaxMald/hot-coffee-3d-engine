#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"

namespace hc::editor
{
  namespace editorServiceManagerRegistry
  {
    void registerServices(EditorServiceManager& serviceManager)
    {
      serviceManager.registerService<ProjectManager>(
        MakeUnique<ProjectManager>()
      );
      serviceManager.registerService<GameObjectSelectionService>(
        MakeUnique<GameObjectSelectionService>()
      );
    }
  }
}
