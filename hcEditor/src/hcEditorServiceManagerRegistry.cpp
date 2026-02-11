#include "hc/editor/hcEditorServiceManagerRegistry.h"
#include "hc/editor/hcEditorServiceManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcProjectManager.h"

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
