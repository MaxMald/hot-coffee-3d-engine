#include "hc/editor/hcEditorServiceManagerRegistry.h"
#include "hc/editor/hcEditorServiceManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"

namespace hc::editor
{
  namespace editorServiceManagerRegistry
  {
    void registerServices(EditorServiceManager& serviceManager)
    {
      serviceManager.registerService<GameObjectSelectionService>(
        MakeUnique<GameObjectSelectionService>()
      );
    }
  }
}
