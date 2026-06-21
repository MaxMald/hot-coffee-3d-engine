#include "hc/editor/views/hcEditorViewsRegistry.h"

#include "hc/editor/editorLogHistory/hcEditorLogHistory.h"
#include "hc/editor/materialDrawer/hcMaterialDrawersManagerFactory.h"

// Services
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"

// Views
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/views/mainMenuBar/hcMainMenuBarFactory.h"
#include "hc/editor/views/mainMenuBar/hcMainMenuBar.h"
#include "hc/editor/views/fileDialog/hcFileDialogView.h"

// Views / Windows
#include "hc/editor/views/windows/hcPluginManagerWindow.h"
#include "hc/editor/views/windows/hcEditorLoggerWindow.h"
#include "hc/editor/views/windows/hcSceneGraphWindow.h"
#include "hc/editor/views/windows/hcProjectBrowserWindow.h"
#include "hc/editor/views/windows/gameObjectEditor/hcGameObjectEditorWindow.h"
#include "hc/editor/views/windows/hcCameraManagerWindow.h"
#include "hc/editor/views/windows/materialDescriptorEditorWindow/hcMaterialDescriptorEditorWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"
#include "hc/editor/views/windows/graphicsWindow/hcGraphicsWindow.h"
#include "hc/editor/views/windows/hcMeshManagerWindow.h"
#include "hc/editor/views/windows/hcMaterialManagerWindow.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"
#include "hc/editor/views/windows/sceneViewportSettings/hcSceneViewportSettingsWindow.h"
#include "hc/editor/views/windows/assetEditors/hcCubeMapDescriptorAssetEditor.h"
#include "hc/editor/views/windows/hcTextureManagerWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcAssetManagerDrawersRegistry.h"
#include "hc/editor/views/windows/sceneSkybox/hcSceneSkyboxWindow.h"

namespace hc::editor
{
  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      HotCoffeeEngine& hotCoffeeEngine,
      EditorViewsManager& viewsManager,
      EditorServiceManager& editorServiceManager,
      EditorLogHistory& editorLogHistory
    )
    {
      viewsManager.registerView(MakeUnique<FileDialogView>());
      viewsManager.registerView(MakeUnique<PluginManagerWindow>(
        hotCoffeeEngine.getPluginManager()
      ));
      viewsManager.registerView(MakeUnique<EditorLoggerWindow>(editorLogHistory));
      viewsManager.registerView(MakeUnique<SceneGraphWindow>(
        hotCoffeeEngine.getSceneManager(),
        editorServiceManager.getService<GameObjectSelectionService>()
      ));

      UniquePtr<SceneViewportWindow> sceneViewportWindow = MakeUnique<SceneViewportWindow>(
        hotCoffeeEngine,
        editorServiceManager.getService<GameObjectSelectionService>()
      );
      viewsManager.registerView(MakeUnique<SceneViewportSettingsWindow>(
        *sceneViewportWindow,
        hotCoffeeEngine.getGraphicsManager()
      ));
      viewsManager.registerView(MakeUnique<CameraManagerWindow>(
        hotCoffeeEngine.getSceneManager()
      ));

      UniquePtr<AssetManagerWindow> assetManagerWindow = MakeUnique<AssetManagerWindow>();
      assetManagerDrawersRegistry::registerAssetGroupDrawers(
        *assetManagerWindow,
        hotCoffeeEngine.getAssetManager()
      );

      viewsManager.registerView(MakeUnique<TextureManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getTextureManager()
      ));
      viewsManager.registerView(MakeUnique<MeshManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getMeshManager()
      ));

      UniquePtr<ProjectFileDialogView> projectFileSelector =
        MakeUnique<ProjectFileDialogView>(editorServiceManager.getService<ProjectManager>());

      UniquePtr<MaterialDescriptorEditorWindow> matDescEditorWindow =
        MakeUnique<MaterialDescriptorEditorWindow>(
          hotCoffeeEngine.getAssetManager(),
          *projectFileSelector
        );

      viewsManager.registerView(MakeUnique<ProjectBrowserWindow>(
        editorServiceManager.getService<ProjectManager>(),
        *matDescEditorWindow
      ));

      viewsManager.registerView(MakeUnique<GameObjectEditorWindow>(
        hotCoffeeEngine,
        *projectFileSelector,
        editorServiceManager.getService<GameObjectSelectionService>()
      ));

      viewsManager.registerView(MakeUnique<MaterialManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getMaterialManager(),
        materialDrawerManagerFactory::create()
      ));
      viewsManager.registerView(MakeUnique<GraphicsWindow>(
        hotCoffeeEngine.getGraphicsManager()
      ));

      viewsManager.registerView(MakeUnique<CubeMapDescriptorAssetEditor>(
        editorServiceManager.getService<ProjectManager>(),
        *projectFileSelector
      ));

      viewsManager.registerView(MakeUnique<SceneSkyboxWindow>(
        editorServiceManager.getService<EditorSceneManager>(),
        *projectFileSelector,
        hotCoffeeEngine.getAssetManager(),
        hotCoffeeEngine.getGraphicsManager()
      ));

      // The order of registration matters for some views, such as the main menu bar which
      // needs to access other views when being created. To ensure that all dependencies
      // are registered before the main menu bar, we register it last after all other
      // views have been registered.

      viewsManager.registerView(std::move(assetManagerWindow));
      viewsManager.registerView(std::move(matDescEditorWindow));
      viewsManager.registerView(std::move(projectFileSelector));
      viewsManager.registerView(std::move(sceneViewportWindow));

      // The main menu bar is registered last to ensure it can access all other views when
      // being created.

      viewsManager.registerView(
        mainMenuBarFactory::create(
          viewsManager, 
          editorServiceManager
        )
      );
    }
  }
}
