#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIProjectManagerListener.h"

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

  protected:
    ProjectManager* m_projectManager;

    void onDraw() override;
  };
}
