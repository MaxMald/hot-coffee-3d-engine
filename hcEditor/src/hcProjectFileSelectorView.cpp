#include "hc/editor/hcProjectFileSelector.h"

#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcFileReference.h"
#include "hc/editor/hcDirectoryReference.h"
#include "imgui.h"

namespace hc::editor
{
  ProjectFileSelector* ProjectFileSelector::s_instance = nullptr;

  ProjectFileSelector& ProjectFileSelector::Instance()
  {
    if (!s_instance)
    {
      throw RuntimeErrorException(
        "ProjectFileSelectorView instance is not prepared. Call Prepare() before accessing the instance."
      );
    }

    return *s_instance;
  }

  void ProjectFileSelector::Prepare()
  {
    if (!s_instance)
    {
      s_instance = new ProjectFileSelector();
      ProjectManager::Instance().subscribeListener(s_instance);
    }
  }

  void ProjectFileSelector::Shutdown()
  {
    if (s_instance)
    {
      ProjectManager::Instance().unsubscribeListener(s_instance);
      delete s_instance;
      s_instance = nullptr;
    }
  }

  void ProjectFileSelector::OpenImageFile(
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    if (s_instance)
    {
      s_instance->openFileSelector(
        "Select Image",
        s_instance->m_imageFileExtensions,
        onFileSelected
      );
    }
  }

  void ProjectFileSelector::OpenModelFile(
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    if (s_instance)
    {
      s_instance->openFileSelector(
        "Select Model",
        s_instance->m_modelFileExtensions,
        onFileSelected
      );
    }
  }

  void ProjectFileSelector::OpenFile(
    const String& title, 
    const Vector<String>& filters, 
    const std::function<void(const Path&)>& onFileSelected
  )
  {
    if (s_instance)
    {
      s_instance->openFileSelector(title, filters, onFileSelected);
    }
  }

  void ProjectFileSelector::OpenDirectory(
    const String& title, 
    const std::function<void(const Path&)>& onDirectorySelected
  )
  {
    if (s_instance)
    {
      s_instance->openDirectorySelector(title, onDirectorySelected);
    }
  }

  ProjectFileSelector::ProjectFileSelector() :
    ABaseView(),
    m_isFileSelectorOpen(false),
    m_isDirectorySelectorOpen(false)
  {
    m_imageFileExtensions = Vector<String>(
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.begin(),
      assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS.end()
    );

    m_modelFileExtensions = Vector<String>(
      assetFileExtensions::SUPPORTED_MODEL_EXTENSIONS.begin(),
      assetFileExtensions::SUPPORTED_MODEL_EXTENSIONS.end()
    );
  }

  ProjectFileSelector::~ProjectFileSelector()
  {
  }

  void ProjectFileSelector::draw()
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

  void ProjectFileSelector::onProjectOpened()
  {
    clear();
    m_directoryNavigator.clear();

    if (!ProjectManager::Instance().isProjectOpen())
      return;

    Path currentProjectDirectory = ProjectManager::Instance().getCurrentProjectDirectory();
    if (currentProjectDirectory.empty())
      return;

    m_directoryNavigator.initialize(currentProjectDirectory);
  }

  void ProjectFileSelector::onProjectClosed()
  {
    clear();
    m_directoryNavigator.clear();
  }

  void ProjectFileSelector::openFileSelector(
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

  void ProjectFileSelector::openDirectorySelector(
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

  void ProjectFileSelector::drawDirectorySelectionInterface()
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

  void ProjectFileSelector::drawFileSelectionInterface()
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

  bool ProjectFileSelector::drawBackAndRefreshButtons()
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

  void ProjectFileSelector::clear()
  {
    m_isFileSelectorOpen = false;
    m_isDirectorySelectorOpen = false;
    m_selectionCallback = nullptr;
    m_currentTitle.clear();
    m_fileFilters.clear();
  }

  void ProjectFileSelector::logWarningIfAlreadyOpen()
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

  bool ProjectFileSelector::isValidFile(const FileReference& file) const
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

  bool ProjectFileSelector::onDirectorySelected(
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

  bool ProjectFileSelector::onFileSelected(
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
