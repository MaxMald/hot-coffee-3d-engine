#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryNavigator.h"

namespace hc::editor
{
  class FileReference;
  class MaterialDescriptorEditorWindow;
  class ProjectManager;

  class ProjectBrowserWindow :
    public AWindowView,
    public IProjectManagerListener
  {
  public:
    ProjectBrowserWindow(
      ProjectManager& projectManager,
      MaterialDescriptorEditorWindow& matDescEditorWindow
    );
    virtual ~ProjectBrowserWindow();

    void onProjectOpened() override;
    void onProjectClosed() override;
    void refresh();

  protected:
    MaterialDescriptorEditorWindow& m_matDescEditorWindow;
    ProjectManager& m_projectManager;
    DirectoryNavigator m_directoryNavigator;

    void onDraw() override;
    void drawDirectoryNavigator();
    void drawAssetCreatorInterface();
    void tryOpenEditorForFile(const FileReference& fileReference);

    Path combineDirectoryWithFileName(
      const Path& directoryPath,
      const String& fileName,
      const String& extension
    );
  };
}
