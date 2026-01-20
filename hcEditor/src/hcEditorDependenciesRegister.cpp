#include "hc/editor/hcEditorDependenciesRegister.h"

#include <hc/hcDependencyContainer.h>

#include "hc/editor/hcGameObjectSelectionService.h"

// Services
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectManager.h"

// Views
#include "hc/editor/hcMainMenuToolbar.h"
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcProjectBrowserWindow.h"
#include "hc/editor/hcGameObjectEditorWindow.h"

namespace hc::editor
{
  namespace editorDependenciesRegister
  {
    void registerDependencies(DependencyContainer& container)
    {
      // Services
      container.registerInstance<GameObjectSelectionService>(MakeShared<GameObjectSelectionService>());
      container.registerInstance<ProjectManager>(MakeShared<ProjectManager>());

      // Views
      container.registerInstance<EditorViewsManager>(MakeShared<EditorViewsManager>());
      container.registerInstance<MainMenuToolbar>(MakeShared<MainMenuToolbar>());
      container.registerInstance<PluginManagerWindow>(MakeShared<PluginManagerWindow>());
      container.registerInstance<EditorLoggerWindow>(MakeShared<EditorLoggerWindow>());
      container.registerInstance<SceneGraphWindow>(MakeShared<SceneGraphWindow>());
      container.registerInstance<ProjectBrowserWindow>(MakeShared<ProjectBrowserWindow>());
      container.registerInstance<GameObjectEditorWindow>(MakeShared<GameObjectEditorWindow>());
    }
  }
}
