#include "hc/editor/hcEditorViewsRegistry.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelector.h"
#include "hc/editor/hcMainMenuToolbar.h"
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcProjectBrowserWindow.h"
#include "hc/editor/hcGameObjectEditorWindow.h"
#include "hc/editor/hcLightManagerWindow.h"
#include "hc/editor/hcCameraManagerWindow.h"
#include "hc/editor/hcMaterialDescriptorEditorWindow.h"
#include "hc/editor/hcAssetManagerWindow.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcEditorLogHistory.h"

namespace hc::editor
{
  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      EditorViewsManager& viewsManager,
      GameObjectSelectionService& gameObjectSelectionService,
      ProjectManager& projectManager,
      EditorLogHistory& editorLogHistory
    )
    {
      viewsManager.registerView(MakeUnique<MainMenuToolbar>(viewsManager, projectManager));
      viewsManager.registerView(MakeUnique<PluginManagerWindow>());
      viewsManager.registerView(MakeUnique<EditorLoggerWindow>(editorLogHistory));
      viewsManager.registerView(MakeUnique<SceneGraphWindow>(gameObjectSelectionService));
      viewsManager.registerView(MakeUnique<LightManagerWindow>());
      viewsManager.registerView(MakeUnique<CameraManagerWindow>());
      viewsManager.registerView(MakeUnique<AssetManagerWindow>());

      UniquePtr<ProjectFileSelector> projectFileSelector =
        MakeUnique<ProjectFileSelector>(projectManager);

      UniquePtr<MaterialDescriptorEditorWindow> matDescEditorWindow =
        MakeUnique<MaterialDescriptorEditorWindow>(*projectFileSelector);

      viewsManager.registerView(MakeUnique<ProjectBrowserWindow>(
        projectManager,
        *matDescEditorWindow
      ));
      viewsManager.registerView(MakeUnique<GameObjectEditorWindow>(
        *projectFileSelector,
        gameObjectSelectionService
      ));

      viewsManager.registerView(std::move(matDescEditorWindow));
      viewsManager.registerView(std::move(projectFileSelector));
    }
  }
}
