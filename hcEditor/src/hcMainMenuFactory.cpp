#include "hc/editor/hcMainMenuBarFactory.h"
#include "hc/editor/hcMenuBuilder.h"
#include "hc/editor/hcMenu.h"
#include "hc/editor/hcMainMenuBar.h"
#include "hc/editor/hcProjectManager.h"

// Windows
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcLightManagerWindow.h"
#include "hc/editor/hcCameraManagerWindow.h"
#include "hc/editor/hcAssetManagerWindow.h"
#include "hc/editor/hcFileDialogView.h"
#include "hc/editor/hcMeshManagerWindow.h"
#include "hc/editor/hcMaterialManagerWindow.h"

// Menu Items
#include "hc/editor/hcOpenProjectMenuItem.h"
#include "hc/editor/hcToggleWindowMenuItem.h"

namespace hc::editor
{
  namespace mainMenuBarFactory
  {
    UniquePtr<MainMenuBar> create(
      EditorViewsManager& editorViewsManager,
      ProjectManager& projectManager
    )
    {
      UniquePtr<MainMenuBar> mainMenuBar = MakeUnique<MainMenuBar>();
      MenuBuilder menuBuilder;

      mainMenuBar->addMenu(
        menuBuilder
          .beginMenu("File")
            .addMenuItem(MakeUnique<OpenProjectMenuItem>(
              projectManager,
              *editorViewsManager.getView<FileDialogView>()
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
              *editorViewsManager.getView<MaterialManagerWindow>()
            ))
            .addMenuItem(MakeUnique<ToggleWindowMenuItem>(
              *editorViewsManager.getView<MeshManagerWindow>()
            ))
          .endMenu()
          .build()
      );

      return std::move(mainMenuBar);
    }
  }
}
