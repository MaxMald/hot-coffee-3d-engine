#include "hc/editor/hcProjectBrowserWindow.h"

#include <filesystem>
#include <hc/hcDependencyContainer.h>
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcDirectoryReference.h"
#include "hc/editor/hcFileReference.h"
#include "imgui.h"

namespace hc::editor
{
  ProjectBrowserWindow::ProjectBrowserWindow() :
    AWindowView("Project Browser", true),
    m_projectManager(nullptr)
  {
  }

  ProjectBrowserWindow::~ProjectBrowserWindow()
  {
  }

  void ProjectBrowserWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_projectManager = container.resolve<ProjectManager>().get();
    m_projectManager->subscribeListener(this);
  }

  void ProjectBrowserWindow::onProjectOpened()
  {
    if (!m_projectManager->isProjectOpen())
      return;

    Path projectPath = m_projectManager->getCurrentProjectPath();
    Path projectDir = projectPath.parent_path();
    m_directoryNavigator.initialize(projectDir);
  }

  void ProjectBrowserWindow::onProjectClosed()
  {
    m_directoryNavigator.clear();
  }

  void ProjectBrowserWindow::onDraw()
  {
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
      return;


    ImGui::Text("Current Directory: %s", currentDir->getFullPath().string().c_str());

    // Back button
    if (ImGui::Button("Back"))
    {
      m_directoryNavigator.navigateToParentDirectory();
      return;
    }

    ImGui::SameLine();

    // Refresh button
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
        // TODO
      }
    }
  }

  void ProjectBrowserWindow::refresh()
  {
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (currentDir)
      currentDir->refresh();
  }
}
