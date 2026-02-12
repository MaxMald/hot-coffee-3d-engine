#include "hc/editor/hcEditorViewsRegistry.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelector.h"
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
#include "hc/editor/hcMainMenuBarFactory.h"
#include "hc/editor/hcMainMenuBar.h"
#include "hc/editor/hcFileDialogView.h"
#include "hc/editor/hcMeshManagerWindow.h"
#include "hc/editor/hcMaterialManagerWindow.h"
#include "hc/editor/hcMaterialDrawersManagerFactory.h"
#include "hc/editor/hcTextureManagerWindow.h"
#include "hc/editor/hcEditorServiceManager.h"
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcAssetGroupDrawersRegistry.h"

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
