#include "hc/editor/hcProjectFileSelectorView.h"

#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcFileReference.h"
#include "hc/editor/hcDirectoryReference.h"
#include "imgui.h"

namespace hc::editor
{
  ProjectFileSelectorView::ProjectFileSelectorView() :
    m_isFileSelectorOpen(false),
    m_isDirectorySelectorOpen(false)
  {
    m_imageFileExtensions = Vector<String>(
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.begin(),
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.end()
    );
  }

  ProjectFileSelectorView::~ProjectFileSelectorView()
  {
  }

  void ProjectFileSelectorView::draw()
  {
    if (m_isDirectorySelectorOpen)
    {
      drawDirectorySelectionInterface();

      if (ImGui::Button("Cancel"))
        clear();
    }
    else if (m_isFileSelectorOpen)
    {
      drawFileSelectionInterface();

      if (ImGui::Button("Cancel"))
        clear();
    }
  }

  void ProjectFileSelectorView::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_projectManager = container.resolve<ProjectManager>();
    m_projectManager->subscribeListener(this);
  }

  void ProjectFileSelectorView::onProjectOpened()
  {
    clear();
    m_directoryNavigator.clear();

    if (!m_projectManager->isProjectOpen())
      return;

    Path currentProjectDirectory = m_projectManager->getCurrentProjectDirectory();
    if (currentProjectDirectory.empty())
      return;

    m_directoryNavigator.initialize(currentProjectDirectory);
  }

  void ProjectFileSelectorView::onProjectClosed()
  {
    clear();
    m_directoryNavigator.clear();
  }

  void ProjectFileSelectorView::openImageFileSelector(
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    clear();
    m_isFileSelectorOpen = true;
    m_currentTitle = "Select Image File";
    m_fileFilters = m_imageFileExtensions;
    m_selectionCallback = onFileSelected;
  }

  void ProjectFileSelectorView::openFileSelector(
    const String& title,
    const Vector<String>& filters,
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    logWarningIfAlreadyOpen();
    clear();
    m_isFileSelectorOpen = true;
    m_currentTitle = (title.empty() ? "Select File" : title);
    m_fileFilters = filters;
    m_selectionCallback = onFileSelected;
  }

  void ProjectFileSelectorView::openDirectorySelector(
    const String& title,
    const std::function<void(const Path&)>& onDirectorySelected
  )
  {
    logWarningIfAlreadyOpen();
    clear();
    m_isDirectorySelectorOpen = true;
    m_currentTitle = (title.empty() ? "Select Directory" : title);
    m_selectionCallback = onDirectorySelected;
  }

  void ProjectFileSelectorView::drawDirectorySelectionInterface()
  {
    if (drawBackAndRefreshButtons())
      return;

    ImGui::Separator();

    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
    {
      ImGui::Text("No Directory Selected");
      return;
    }

    if (ImGui::Button("Select this Directory"))
    {
      onDirectorySelected(*currentDir);
      return;
    }

    for (const auto& subDir : currentDir->getSubDirectories())
    {
      String displayName = "<folder> " + subDir->getName();
      if (ImGui::Selectable(displayName.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());
        return;
      }
    }
  }

  void ProjectFileSelectorView::drawFileSelectionInterface()
  {
    if (drawBackAndRefreshButtons())
      return;

    ImGui::Separator();

    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
    {
      ImGui::Text("No Directory Selected");
      return;
    }

    for (const auto& subDir : currentDir->getSubDirectories())
    {
      String displayName = "<folder> " + subDir->getName();
      if (ImGui::Selectable(displayName.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());
        return;
      }
    }

    for (const auto& file : currentDir->getFiles())
    {
      if (!isValidFile(*file))
        continue;

      if (ImGui::Selectable(file->getNameWithExtension().c_str()))
      {
        if (onFileSelected(*file))
          return;
      }
    }
  }

  bool ProjectFileSelectorView::drawBackAndRefreshButtons()
  {
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
      return false;

    if (ImGui::Button("Back"))
    {
      m_directoryNavigator.navigateToParentDirectory();
      return true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
    {
      currentDir->refresh();
      return true;
    }

    return false;
  }

  void ProjectFileSelectorView::clear()
  {
    m_isFileSelectorOpen = false;
    m_isDirectorySelectorOpen = false;
    m_selectionCallback = nullptr;
    m_currentTitle.clear();
    m_fileFilters.clear();
  }

  void ProjectFileSelectorView::logWarningIfAlreadyOpen()
  {
    if (m_isFileSelectorOpen)
    {
      LogService::Warning(
        String::Format(
          "File selector is already open for a windows with title: %s. The file selector will be aborted.",
          m_currentTitle.c_str()
        )
      );
    }

    if (m_isDirectorySelectorOpen)
    {
      LogService::Warning(
        String::Format(
          "Directory selector is already open for a windows with title: %s. The directory selector will be aborted.",
          m_currentTitle.c_str()
        )
      );
    }
  }

  bool ProjectFileSelectorView::isValidFile(const FileReference& file) const
  {
    if (m_fileFilters.empty())
      return true;

    const String& fileExtension = file.getExtension();
    for (const auto& filter : m_fileFilters)
    {
      if (fileExtension == filter)
        return true;
    }

    return false;
  }

  bool ProjectFileSelectorView::onDirectorySelected(
    const DirectoryReference& directory
  )
  {
    if (!m_isDirectorySelectorOpen)
      return false;

    if (m_selectionCallback)
      m_selectionCallback(directory.getFullPath());

    clear();
    return true;
  }

  bool ProjectFileSelectorView::onFileSelected(
    const FileReference& file
  )
  {
    if (!m_isFileSelectorOpen)
      return false;

    if (m_selectionCallback)
      m_selectionCallback(file.getFullPath());

    clear();
    return true;
  }
}
