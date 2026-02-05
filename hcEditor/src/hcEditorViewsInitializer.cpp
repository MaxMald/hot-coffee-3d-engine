#include "hc/editor/hcEditorViewsInitializer.h"
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
  namespace editorViewsInitializer
  {
    void registerDefaultViews()
    {
      EditorViewsManager& viewsManager = EditorViewsManager::Instance();

      viewsManager.registerView(MakeUnique<ProjectFileSelector>());
      viewsManager.registerView(MakeUnique<MainMenuToolbar>());
      viewsManager.registerView(MakeUnique<PluginManagerWindow>());
      viewsManager.registerView(MakeUnique<EditorLoggerWindow>());
      viewsManager.registerView(MakeUnique<SceneGraphWindow>());
      viewsManager.registerView(MakeUnique<ProjectBrowserWindow>());
      viewsManager.registerView(MakeUnique<GameObjectEditorWindow>());
      viewsManager.registerView(MakeUnique<LightManagerWindow>());
      viewsManager.registerView(MakeUnique<CameraManagerWindow>());
      viewsManager.registerView(MakeUnique<MaterialDescriptorEditorWindow>());
      viewsManager.registerView(MakeUnique<AssetManagerWindow>());
    }
  }
}
