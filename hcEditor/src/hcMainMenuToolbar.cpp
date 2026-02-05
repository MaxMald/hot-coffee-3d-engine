#include "hc/editor/hcMainMenuToolbar.h"

#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcLightManagerWindow.h"
#include "hc/editor/hcCameraManagerWindow.h"
#include "hc/editor/hcAssetManagerWindow.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

namespace hc::editor
{
  static constexpr const char* PROJECT_FILE_EXTENSION = ".hotCoffeeProj";
  static constexpr const char* OPEN_PROJECT_DIALOG_KEY = "OpenProject";

  MainMenuToolbar::MainMenuToolbar(EditorViewsManager* editorViewsManager) :
    ABaseView(),
    m_editorViewsManager(editorViewsManager)
  {
  }

  MainMenuToolbar::~MainMenuToolbar()
  {
  }

  void MainMenuToolbar::draw()
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New Project"))
        {
          // Handle new project action
        }
        if (ImGui::MenuItem("Open Project"))
        {
          IGFD::FileDialogConfig config;
          config.path = ".";

          ImGuiFileDialog::Instance()->OpenDialog(
            OPEN_PROJECT_DIALOG_KEY,
            "Choose Project",
            PROJECT_FILE_EXTENSION,
            config
          );
        }
        if (ImGui::MenuItem("Save Project"))
        {
          // Handle save project action
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit"))
      {
        if (ImGui::MenuItem("Undo"))
        {
          // Handle undo action
        }
        if (ImGui::MenuItem("Redo"))
        {
          // Handle redo action
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Windows"))
      {
        if (ImGui::MenuItem("Plugin Manager"))
          m_editorViewsManager->getView<PluginManagerWindow>()->setOpen(true);

        if (ImGui::MenuItem("Logger"))
          m_editorViewsManager->getView<EditorLoggerWindow>()->setOpen(true);

        if (ImGui::MenuItem("Scene Graph"))
          m_editorViewsManager->getView<SceneGraphWindow>()->setOpen(true);

        if (ImGui::MenuItem("Light Manager"))
          m_editorViewsManager->getView<LightManagerWindow>()->setOpen(true);

        if (ImGui::MenuItem("Camera Manager"))
          m_editorViewsManager->getView<CameraManagerWindow>()->setOpen(true);

        if (ImGui::MenuItem("Asset Manager"))
          m_editorViewsManager->getView<AssetManagerWindow>()->setOpen(true);

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Help"))
      {
        if (ImGui::MenuItem("About"))
        {
          // Handle about action
        }
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    displayOpenProjectDialog();
  }

  void MainMenuToolbar::displayOpenProjectDialog()
  {
    if (ImGuiFileDialog::Instance()->Display(OPEN_PROJECT_DIALOG_KEY)) {
      if (ImGuiFileDialog::Instance()->IsOk())
      {
        String filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        String filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        ProjectManager::Instance().openProject(Path(filePathName.c_str()));
      }

      ImGuiFileDialog::Instance()->Close();
    }
  }
}
