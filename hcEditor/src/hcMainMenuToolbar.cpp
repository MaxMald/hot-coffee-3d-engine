#include "hc/editor/hcMainMenuToolbar.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcSceneManager.h>
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "imgui.h"

namespace hc::editor
{
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
          // Handle open project action
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
  }

  void MainMenuToolbar::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<EditorViewsManager> editorViewsManager = container.resolve<EditorViewsManager>();
    editorViewsManager->registerView(this);

    m_pluginManagerWindow = container.resolve<PluginManagerWindow>();
    m_editorLoggerWindow = container.resolve<EditorLoggerWindow>();
    m_sceneGraphWindow = container.resolve<SceneGraphWindow>();
  }
}
