#include "hc/editor/hcProjectBrowserWindow.h"

#include <hc/hcDependencyContainer.h>
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorViewsManager.h"

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
  }

  void ProjectBrowserWindow::onProjectOpened()
  {
    // TODO
  }

  void ProjectBrowserWindow::onProjectClosed()
  {
    // TODO
  }

  void ProjectBrowserWindow::onDraw()
  {
    // TODO
  }
}
