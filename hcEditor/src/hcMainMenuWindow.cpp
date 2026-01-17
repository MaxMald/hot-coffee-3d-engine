#include "hc/editor/hcMainMenuWindow.h"

#include <hc/hcHotCoffeeEngine.h>

#include "imgui.h"


namespace hc::editor
{
  MainMenuWindow::MainMenuWindow(HotCoffeeEngine& engine) :
    m_pluginManagerWindow(engine.getPluginManager())
  {
  }

  MainMenuWindow::~MainMenuWindow()
  {
  }

  void MainMenuWindow::draw()
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
          m_pluginManagerWindow.setOpen(true);

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

    m_pluginManagerWindow.draw();
  }
}
