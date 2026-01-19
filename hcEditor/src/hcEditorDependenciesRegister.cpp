#include "hc/editor/hcEditorDependenciesRegister.h"

#include <hc/hcDependencyContainer.h>

#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcMainMenuWindow.h"
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"

namespace hc::editor
{
  namespace editorDependenciesRegister
  {
    void registerDependencies(DependencyContainer& container)
    {
      // Services
      container.registerInstance<GameObjectSelectionService>(MakeShared<GameObjectSelectionService>());
      
      // Views
      container.registerInstance<EditorViewsManager>(MakeShared<EditorViewsManager>());
      container.registerInstance<MainMenuWindow>(MakeShared<MainMenuWindow>());
      container.registerInstance<PluginManagerWindow>(MakeShared<PluginManagerWindow>());
      container.registerInstance<EditorLoggerWindow>(MakeShared<EditorLoggerWindow>());
      container.registerInstance<SceneGraphWindow>(MakeShared<SceneGraphWindow>());
    }
  }
}
