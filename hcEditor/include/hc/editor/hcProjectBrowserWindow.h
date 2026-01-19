#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIProjectManagerListener.h"
#include "hc/editor/hcDirectoryNavigator.h"

namespace hc::editor
{
  class ProjectManager;

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
    ProjectManager* m_projectManager;
    DirectoryNavigator m_directoryNavigator;

    void onDraw() override;
  };
}
