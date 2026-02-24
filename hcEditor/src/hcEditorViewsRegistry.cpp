#include "hc/editor/views/hcEditorViewsRegistry.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/hcProjectFileSelector.h"
#include "hc/editor/views/windows/hcPluginManagerWindow.h"
#include "hc/editor/views/windows/hcEditorLoggerWindow.h"
#include "hc/editor/views/windows/hcSceneGraphWindow.h"
#include "hc/editor/views/windows/hcProjectBrowserWindow.h"
#include "hc/editor/views/windows/gameObjectEditor/hcGameObjectEditorWindow.h"
#include "hc/editor/views/windows/hcLightManagerWindow.h"
#include "hc/editor/views/windows/hcCameraManagerWindow.h"
#include "hc/editor/views/windows/materialDescriptorEditorWindow/hcMaterialDescriptorEditorWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistory.h"
#include "hc/editor/views/mainMenuBar/hcMainMenuBarFactory.h"
#include "hc/editor/views/mainMenuBar/hcMainMenuBar.h"
#include "hc/editor/views/fileDialog/hcFileDialogView.h"
#include "hc/editor/views/windows/hcMeshManagerWindow.h"
#include "hc/editor/views/windows/hcMaterialManagerWindow.h"
#include "hc/editor/materialDrawer/hcMaterialDrawersManagerFactory.h"
#include "hc/editor/views/windows/hcTextureManagerWindow.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcAssetGroupDrawersRegistry.h"

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
      viewsManager.registerView(MakeUnique<LightManagerWindow>(
        hotCoffeeEngine.getSceneManager()
      ));
      viewsManager.registerView(MakeUnique<CameraManagerWindow>(
        hotCoffeeEngine.getSceneManager()
      ));

      UniquePtr<AssetManagerWindow> assetManagerWindow = MakeUnique<AssetManagerWindow>();
      assetGroupDrawersRegistry::registerAssetGroupDrawers(
        *assetManagerWindow,
        hotCoffeeEngine.getAssetManager()
      );

      viewsManager.registerView(std::move(assetManagerWindow));
      viewsManager.registerView(MakeUnique<TextureManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getTextureManager()
      ));
      viewsManager.registerView(MakeUnique<MeshManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getMeshManager()
      ));

      UniquePtr<ProjectFileSelector> projectFileSelector =
        MakeUnique<ProjectFileSelector>(editorServiceManager.getService<ProjectManager>());

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
      viewsManager.registerView(std::move(matDescEditorWindow));
      viewsManager.registerView(std::move(projectFileSelector));

      viewsManager.registerView(MakeUnique<MaterialManagerWindow>(
        hotCoffeeEngine.getGraphicsManager().getMaterialManager(),
        materialDrawerManagerFactory::create()
      ));

      viewsManager.registerView(
        mainMenuBarFactory::create(
          viewsManager, 
          editorServiceManager.getService<ProjectManager>()
        )
      );
    }
  }
}
