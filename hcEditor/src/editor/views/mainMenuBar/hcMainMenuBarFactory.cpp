#include "hc/editor/views/mainMenuBar/hcMainMenuBarFactory.h"
#include "hc/editor/views/mainMenuBar/hcMenuBuilder.h"
#include "hc/editor/views/mainMenuBar/hcMenu.h"
#include "hc/editor/views/mainMenuBar/hcMainMenuBar.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

// Windows
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/windows/hcPluginManagerWindow.h"
#include "hc/editor/views/windows/hcEditorLoggerWindow.h"
#include "hc/editor/views/windows/hcSceneGraphWindow.h"
#include "hc/editor/views/windows/hcLightManagerWindow.h"
#include "hc/editor/views/windows/hcCameraManagerWindow.h"
#include "hc/editor/views/windows/assetManagerWindow/hcAssetManagerWindow.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"
#include "hc/editor/views/fileDialog/hcFileDialogView.h"
#include "hc/editor/views/windows/hcMeshManagerWindow.h"
#include "hc/editor/views/windows/hcMaterialManagerWindow.h"
#include "hc/editor/views/windows/hcTextureManagerWindow.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

// Menu Items
#include "hc/editor/views/mainMenuBar/hcOpenProjectMenuItem.h"
#include "hc/editor/views/mainMenuBar/hcSaveProjectMenuItem.h"
#include "hc/editor/views/mainMenuBar/hcToggleWindowMenuItem.h"
#include "hc/editor/views/mainMenuBar/hcSaveSceneMenuItem.h"
#include "hc/editor/views/mainMenuBar/hcOpenSceneMenuItem.h"

namespace hc::editor
{
  namespace mainMenuBarFactory
  {
    UniquePtr<MainMenuBar> create(
      EditorViewsManager& editorViewsManager,
      EditorServiceManager& editorServiceManager
    )
    {
      UniquePtr<MainMenuBar> mainMenuBar = MakeUnique<MainMenuBar>();
      MenuBuilder menuBuilder;

      mainMenuBar->addMenu(
        menuBuilder
          .beginMenu("File")
            .addMenuItem(MakeUnique<SaveProjectMenuItem>(
              editorServiceManager.getService<ProjectManager>(),
              *editorViewsManager.getView<FileDialogView>()
            ))
            .addMenuItem(MakeUnique<OpenProjectMenuItem>(
              editorServiceManager.getService<ProjectManager>(),
              *editorViewsManager.getView<FileDialogView>()
            ))
          .endMenu()
          .build()
      );

      mainMenuBar->addMenu(
        menuBuilder
          .beginMenu("Scene")
            .addMenuItem(MakeUnique<OpenSceneMenuItem>(
              editorServiceManager.getService<ProjectManager>(),
              editorServiceManager.getService<EditorSceneManager>(),
              *editorViewsManager.getView<ProjectFileDialogView>()
            ))
            .addMenuItem(MakeUnique<SaveSceneMenuItem>(
              editorServiceManager.getService<ProjectManager>(),
              editorServiceManager.getService<EditorSceneManager>(),
              *editorViewsManager.getView<ProjectFileDialogView>()
            ))
          .endMenu()
          .build()
      );

      mainMenuBar->addMenu(
        menuBuilder
          .beginMenu("Windows")
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<PluginManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<EditorLoggerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<SceneGraphWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<LightManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<CameraManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<AssetManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<TextureManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<MaterialManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<MeshManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<SceneViewportWindow>()
            ))
          .endMenu()
          .build()
      );

      return std::move(mainMenuBar);
    }
  }
}
