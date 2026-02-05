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

namespace hc::editor
{
  namespace editorViewsRegistry
  {
    void registerDefaultViews(EditorViewsManager& viewsManager)
    {
      viewsManager.registerView(MakeUnique<MainMenuToolbar>(&viewsManager));
      viewsManager.registerView(MakeUnique<PluginManagerWindow>());
      viewsManager.registerView(MakeUnique<EditorLoggerWindow>());
      viewsManager.registerView(MakeUnique<SceneGraphWindow>());
      viewsManager.registerView(MakeUnique<LightManagerWindow>());
      viewsManager.registerView(MakeUnique<CameraManagerWindow>());
      viewsManager.registerView(MakeUnique<AssetManagerWindow>());

      UniquePtr<ProjectFileSelector> projectFileSelector =
        MakeUnique<ProjectFileSelector>();

      UniquePtr<MaterialDescriptorEditorWindow> matDescEditorWindow =
        MakeUnique<MaterialDescriptorEditorWindow>(*projectFileSelector);

      viewsManager.registerView(MakeUnique<ProjectBrowserWindow>(*matDescEditorWindow));
      viewsManager.registerView(MakeUnique<GameObjectEditorWindow>(*projectFileSelector));
      viewsManager.registerView(std::move(matDescEditorWindow));
      viewsManager.registerView(std::move(projectFileSelector));
    }
  }
}
