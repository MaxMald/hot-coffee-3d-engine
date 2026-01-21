#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIProjectManagerListener.h"
#include "hc/editor/hcDirectoryNavigator.h"

namespace hc::editor
{
  class ProjectManager;
  class MaterialDescriptorEditorWindow;
  class FileReference;

  class ProjectBrowserWindow :
    public AWindowView,
    public IDependencyResolvable,
    public IProjectManagerListener
  {
  public:
    ProjectBrowserWindow();
    virtual ~ProjectBrowserWindow();

    void resolveDependencies(DependencyContainer& container) override;
    void onProjectOpened() override;
    void onProjectClosed() override;
    void refresh();

  protected:
    SharedPtr<ProjectManager> m_projectManager;
    SharedPtr<MaterialDescriptorEditorWindow> m_materialDescriptorEditorWindow;
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
