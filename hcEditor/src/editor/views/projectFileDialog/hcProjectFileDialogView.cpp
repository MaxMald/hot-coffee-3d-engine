#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/directoryNavigator/hcFileReference.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryReference.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include <imgui.h>

namespace hc::editor
{
  static constexpr const char* DIRECTORY_PREFIX = "[Dir] ";
  static constexpr const char* FILE_PREFIX = "[File] ";

  ProjectFileDialogView::ProjectFileDialogView(
    ProjectManager& projectManager
  ) :
    m_isFileSelectorOpen(false),
    m_isDirectorySelectorOpen(false),
    m_projectManager(projectManager),
    m_createNewFileUI()
  {
    m_imageFileExtensions = Vector<String>(
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.begin(),
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.end()
    );

    m_modelFileExtensions = Vector<String>(
      assetFileExtensions::SUPPORTED_MODEL_EXTENSIONS.begin(),
      assetFileExtensions::SUPPORTED_MODEL_EXTENSIONS.end()
    );

    m_projectManager.subscribeListener(this);
  }

  ProjectFileDialogView::~ProjectFileDialogView()
  {
    m_projectManager.unsubscribeListener(this);
  }

  void ProjectFileDialogView::update(const Time&)
  {
  }

  void ProjectFileDialogView::draw()
  {
    if (!m_isDirectorySelectorOpen && !m_isFileSelectorOpen)
      return;

    if (ImGui::Begin(m_currentTitle.c_str()))
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

      ImGui::End();
    }
  }

  void ProjectFileDialogView::destroy()
  {
    clear();
  }

  void ProjectFileDialogView::openImageFile(
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    openFileSelector(
      "Select Image",
      m_imageFileExtensions,
      onFileSelected
    );
  }

  void ProjectFileDialogView::openModelFile(
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    openFileSelector(
      "Select Model",
      m_modelFileExtensions,
      onFileSelected
    );
  }

  void ProjectFileDialogView::onProjectOpened()
  {
    clear();
    m_directoryNavigator.clear();

    if (!m_projectManager.isProjectOpen())
      return;

    Path currentProjectDirectory = m_projectManager.getCurrentProjectDirectory();
    if (currentProjectDirectory.empty())
      return;

    m_directoryNavigator.initialize(currentProjectDirectory);
  }

  void ProjectFileDialogView::onProjectClosed()
  {
    clear();
    m_directoryNavigator.clear();
  }

  void ProjectFileDialogView::openFileSelector(
    const String& title,
    const Vector<String>& filters,
    const std::function<void(const Path&)>& onFileSelected,
    bool allowCreateNewFile
  )
  {
    logWarningIfAlreadyOpen();
    clear();

    m_isFileSelectorOpen = true;
    m_currentTitle = (title.empty() ? "Select File" : title);
    m_fileFilters = filters;
    m_selectionCallback = onFileSelected;

    if (allowCreateNewFile && !filters.empty())
    {
      m_createNewFileUI.initialize(
        m_directoryNavigator.getCurrentDirectory()->getFullPath(),
        filters,
        [this](const Path& newFilePath)
        {
          if (m_selectionCallback)
            m_selectionCallback(newFilePath);
          clear();
        }
      );
    }
  }

  void ProjectFileDialogView::openDirectorySelector(
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

  void ProjectFileDialogView::drawDirectorySelectionInterface()
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
      String displayDirectory = DIRECTORY_PREFIX + subDir->getName();
      if (ImGui::Selectable(displayDirectory.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());
        return;
      }
    }
  }

  void ProjectFileDialogView::drawFileSelectionInterface()
  {
    if (m_createNewFileUI.isInitialized())
    {
      m_createNewFileUI.draw();
      if (m_createNewFileUI.wasFileCreated())
        return;

      ImGui::Separator();
    }

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
      String displayDirectoryName = DIRECTORY_PREFIX + subDir->getName();
      if (ImGui::Selectable(displayDirectoryName.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());

        if (m_createNewFileUI.isInitialized())
          m_createNewFileUI.setTargetDirectory(subDir->getFullPath());
        return;
      }
    }

    for (const auto& file : currentDir->getFiles())
    {
      if (!isValidFile(*file))
        continue;

      String displayFileName = FILE_PREFIX + file->getNameWithExtension();
      if (ImGui::Selectable(displayFileName.c_str()))
      {
        if (onFileSelected(*file))
          return;
      }
    }
  }

  bool ProjectFileDialogView::drawBackAndRefreshButtons()
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

  void ProjectFileDialogView::clear()
  {
    m_isFileSelectorOpen = false;
    m_isDirectorySelectorOpen = false;
    m_selectionCallback = nullptr;
    m_currentTitle.clear();
    m_fileFilters.clear();
    m_createNewFileUI.reset();
  }

  void ProjectFileDialogView::logWarningIfAlreadyOpen()
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

  bool ProjectFileDialogView::isValidFile(const FileReference& file) const
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

  bool ProjectFileDialogView::onDirectorySelected(
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

  bool ProjectFileDialogView::onFileSelected(
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
