#include "hc/editor/hcMainMenuToolbar.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcSceneManager.h>
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcLightManagerWindow.h"
#include "hc/editor/hcCameraManagerWindow.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

namespace hc::editor
{
  static constexpr const char* PROJECT_FILE_EXTENSION = ".hotCoffeeProj";
  static constexpr const char* OPEN_PROJECT_DIALOG_KEY = "OpenProject";

  MainMenuToolbar::MainMenuToolbar() :
    m_pluginManagerWindow(nullptr),
    m_editorLoggerWindow(nullptr),
    m_sceneGraphWindow(nullptr)
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
          m_pluginManagerWindow->setOpen(true);

        if (ImGui::MenuItem("Logger"))
          m_editorLoggerWindow->setOpen(true);

        if (ImGui::MenuItem("Scene Graph"))
          m_sceneGraphWindow->setOpen(true);

        if (ImGui::MenuItem("Light Manager"))
          m_lightManagerWindow->setOpen(true);

        if (ImGui::MenuItem("Camera Manager"))
          m_cameraManagerWindow->setOpen(true);

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

  void MainMenuToolbar::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<EditorViewsManager> editorViewsManager = container.resolve<EditorViewsManager>();
    editorViewsManager->registerView(this);

    m_pluginManagerWindow = container.resolve<PluginManagerWindow>();
    m_editorLoggerWindow = container.resolve<EditorLoggerWindow>();
    m_sceneGraphWindow = container.resolve<SceneGraphWindow>();
    m_projectManager = container.resolve<ProjectManager>();
    m_lightManagerWindow = container.resolve<LightManagerWindow>();
    m_cameraManagerWindow = container.resolve<CameraManagerWindow>();
  }

  void MainMenuToolbar::displayOpenProjectDialog()
  {
    if (ImGuiFileDialog::Instance()->Display(OPEN_PROJECT_DIALOG_KEY)) {
      if (ImGuiFileDialog::Instance()->IsOk())
      {
        String filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        String filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        m_projectManager->openProject(Path(filePathName.c_str()));
      }

      ImGuiFileDialog::Instance()->Close();
    }
  }
}
