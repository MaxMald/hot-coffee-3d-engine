#include "hc/editor/views/windows/hcProjectBrowserWindow.h"

#include <filesystem>
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryReference.h"
#include "hc/editor/views/directoryNavigator/hcFileReference.h"
#include "imgui.h"

namespace hc::editor
{
  ProjectBrowserWindow::ProjectBrowserWindow(
    ProjectManager& projectManager
  ) :
    AWindowView("Project Browser", true),
    m_projectManager(projectManager)
  {
    m_projectManager.subscribeListener(this);
  }

  ProjectBrowserWindow::~ProjectBrowserWindow()
  {
    m_projectManager.unsubscribeListener(this);
  }

  void ProjectBrowserWindow::destroy()
  {
    m_directoryNavigator.clear();
  }

  void ProjectBrowserWindow::onProjectOpened()
  {
    if (!m_projectManager.isProjectOpen())
      return;

    Path projectPath = m_projectManager.getCurrentProjectPath();
    Path projectDir = projectPath.parent_path();
    m_directoryNavigator.initialize(projectDir);
  }

  void ProjectBrowserWindow::onProjectClosed()
  {
    m_directoryNavigator.clear();
  }

  void ProjectBrowserWindow::onDraw()
  {
    drawDirectoryNavigator();
  }

  void ProjectBrowserWindow::drawDirectoryNavigator()
  {
    ImGui::Text("Project Navigator");
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
    {
      ImGui::Text("No directory selected. Load a Project first.");
      return;
    }
    
    ImGui::Text("Current Directory: %s", currentDir->getFullPath().string().c_str());
    if (ImGui::Button("Back"))
    {
      m_directoryNavigator.navigateToParentDirectory();
      return;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
    {
      refresh();
      return;
    }
    ImGui::Separator();

    // Folders
    for (const auto& subDir : currentDir->getSubDirectories())
    {
      std::string displayName = "<folder> " + subDir->getName();
      if (ImGui::Selectable(displayName.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());
        return;
      }
    }

    // Files
    for (const auto& file : currentDir->getFiles())
    {
      if (ImGui::Selectable(file->getNameWithExtension().c_str()))
      {
        // TODO: Should open a viewer or editor for the selected file based on its type.
      }
    }
  }

  Path ProjectBrowserWindow::combineDirectoryWithFileName(
    const Path& directoryPath,
    const String& fileName,
    const String& extension
  )
  {
    Path fullPath = directoryPath / (fileName + extension);
    return fullPath;
  }

  void ProjectBrowserWindow::refresh()
  {
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (currentDir)
      currentDir->refresh();
  }
}
