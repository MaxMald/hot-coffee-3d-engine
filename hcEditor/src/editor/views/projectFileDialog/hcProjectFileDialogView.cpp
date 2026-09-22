#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/directoryNavigator/hcFileReference.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryReference.h"
#include "hc/editor/views/projectFileDialog/hcContextProjectFileDialogRequest.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogRequest.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include <imgui.h>

namespace hc::editor
{
  static constexpr const char* DIRECTORY_PREFIX = "[Dir] ";
  static constexpr const char* FILE_PREFIX = "[File] ";

  ProjectFileDialogView::ProjectFileDialogView(
    ProjectManager& projectManager
  ) :
    m_projectManager(projectManager),
    m_currentRequest(nullptr),
    m_isFileSelectorOpen(false),
    m_isDirectorySelectorOpen(false),
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
    if (!m_isDirectorySelectorOpen && !m_isFileSelectorOpen && !m_currentRequest)
      return;

    if (ImGui::Begin(m_currentRequest->getTitle().c_str()))
    {
      if (m_isDirectorySelectorOpen)
      {
        drawDirectorySelectionInterface();

        if (ImGui::Button("Cancel"))
          cancel();
      }
      else if (m_isFileSelectorOpen)
      {
        drawFileSelectionInterface();

        if (ImGui::Button("Cancel"))
          cancel();
      }

      ImGui::End();
    }
  }

  void ProjectFileDialogView::destroy()
  {
    m_isFileSelectorOpen = false;
    m_isDirectorySelectorOpen = false;

    if (m_currentRequest != nullptr)
    {
      m_currentRequest->destroy();
      m_currentRequest.reset();
    }

    m_createNewFileUI.reset();
  }

  void ProjectFileDialogView::openImageFile(
    const std::function<void(const Path&)>& onFileSelected,
    const std::function<void()>& onCancel
  )
  {
    openFileSelector(
      "Select Image",
      m_imageFileExtensions,
      onFileSelected,
      onCancel
    );
  }

  void  ProjectFileDialogView::openImageFile(
    const std::function<void(const Path&, void*)>& onFileSelected,
    const std::function<void(void*)>& onCancel,
    const std::function<void(void*)>& destroyContext,
    void* context
  )
  {
    openFileSelector(
      "Select Image",
      m_imageFileExtensions,
      onFileSelected,
      onCancel,
      destroyContext,
      context
    );
  }

  void ProjectFileDialogView::openModelFile(
    const std::function<void(const Path&)>& onFileSelected,
    const std::function<void()>& onCancel
  )
  {
    openFileSelector(
      "Select Model",
      m_modelFileExtensions,
      onFileSelected,
      onCancel
    );
  }

  void ProjectFileDialogView::openFileSelector(
    const String& title,
    const Vector<String> filters,
    const std::function<void(const Path&, void*)>& onFileSelected,
    const std::function<void(void*)>& onCancel,
    const std::function<void(void*)>& destroyContext,
    void* context,
    bool allowCreateNewFile
  )
  {
    logWarningIfAlreadyOpen();
    cancel();

    m_isFileSelectorOpen = true;
    m_currentRequest = MakeUnique<projectFileDialog::ContextProjectFileDialogRequest>(
      (title.empty() ? "Select File" : title),
      filters,
      onFileSelected,
      onCancel,
      destroyContext,
      context
    );

    if (allowCreateNewFile && !filters.empty())
    {
      m_createNewFileUI.initialize(
        m_directoryNavigator.getCurrentDirectory()->getFullPath(),
        filters,
        [this](const Path& newFilePath)
        {
          if (m_currentRequest != nullptr)
          {
            m_currentRequest->onFileSelected(newFilePath);
            m_currentRequest->destroy();
            m_currentRequest.reset();
          }
        }
      );
    }
  }

  void ProjectFileDialogView::openFileSelector(
    const String& title,
    const Vector<String>& filters,
    const std::function<void(const Path&)>& onFileSelected,
    const std::function<void()>& onCancel,
    bool allowCreateNewFile
  )
  {
    logWarningIfAlreadyOpen();
    cancel();

    m_isFileSelectorOpen = true;
    m_currentRequest = MakeUnique<projectFileDialog::ProjectFileDialogRequest>(
      (title.empty() ? "Select File" : title),
      filters,
      onFileSelected,
      onCancel
    );

    if (allowCreateNewFile && !filters.empty())
    {
      m_createNewFileUI.initialize(
        m_directoryNavigator.getCurrentDirectory()->getFullPath(),
        filters,
        [this](const Path& newFilePath)
        {
          if (m_currentRequest != nullptr)
          {
            m_currentRequest->onFileSelected(newFilePath);
            m_currentRequest->destroy();
            m_currentRequest.reset();
          }
        }
      );
    }
  }

  void ProjectFileDialogView::openDirectorySelector(
    const String& title,
    const std::function<void(const Path&)>& onDirectorySelected,
    const std::function<void()>& onCancel
  )
  {
    logWarningIfAlreadyOpen();
    cancel();

    m_isDirectorySelectorOpen = true;
    m_currentRequest = MakeUnique<projectFileDialog::ProjectFileDialogRequest>(
      (title.empty() ? "Select Directory" : title),
      Vector<String>(),
      onDirectorySelected,
      onCancel
    );
  }

  void ProjectFileDialogView::onProjectOpened()
  {
    cancel();
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
    cancel();
    m_directoryNavigator.clear();
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

  void ProjectFileDialogView::cancel()
  {
    m_isFileSelectorOpen = false;
    m_isDirectorySelectorOpen = false;

    if (m_currentRequest != nullptr)
    {
      m_currentRequest->onCancel();
      m_currentRequest->destroy();
      m_currentRequest.reset();
    }

    m_createNewFileUI.reset();
  }

  void ProjectFileDialogView::logWarningIfAlreadyOpen()
  {
    String currentTitle = "";
    if (m_currentRequest != nullptr)
      currentTitle = m_currentRequest->getTitle();

    if (m_isFileSelectorOpen)
    {
      LogService::Warning(
        String::Format(
          "File selector is already open for a window with title: %s. The file selector will be aborted.",
          currentTitle.c_str()
        )
      );
    }

    if (m_isDirectorySelectorOpen)
    {
      LogService::Warning(
        String::Format(
          "Directory selector is already open for a windows with title: %s. The directory selector will be aborted.",
          currentTitle.c_str()
        )
      );
    }
  }

  bool ProjectFileDialogView::isValidFile(const FileReference& file) const
  {
    if (m_currentRequest == nullptr)
      return false;

    if (m_currentRequest->getFilters().empty())
      return true;

    const String& fileExtension = file.getExtension();
    for (const auto& filter : m_currentRequest->getFilters())
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

    if (m_currentRequest != nullptr)
    {
      m_currentRequest->onFileSelected(directory.getFullPath());
      m_currentRequest->destroy();
      m_currentRequest.reset();
    }
    
    return true;
  }

  bool ProjectFileDialogView::onFileSelected(
    const FileReference& file
  )
  {
    if (!m_isFileSelectorOpen)
      return false;

    if (m_currentRequest != nullptr)
    {
      m_currentRequest->onFileSelected(file.getFullPath());
      m_currentRequest->destroy();
      m_currentRequest.reset();
    }

    return true;
  }
}
