#include "hc/editor/hcProjectManager.h"

#include "hc/editor/hcProject.h"

namespace hc::editor
{
  ProjectManager::ProjectManager() :
    m_currentProjectPath(),
    m_isProjectOpen(false),
    m_currentProject(nullptr)
  {
  }

  ProjectManager::~ProjectManager()
  {
  }

  bool ProjectManager::openProject(const Path& projectPath)
  {
    closeProject();

    // TODO load project
    m_currentProjectPath = projectPath;
    m_isProjectOpen = true;
    return true;
  }

  bool ProjectManager::closeProject()
  {
    if (m_isProjectOpen && m_currentProject)
    {
      m_currentProject = nullptr;
      m_isProjectOpen = false;
      m_currentProjectPath.clear();
      return true;
    }

    return false;
  }

  Path ProjectManager::getCurrentProjectPath() const
  {
    return m_currentProjectPath;
  }

  bool ProjectManager::isProjectOpen() const
  {
    return m_isProjectOpen;
  }

  Project* ProjectManager::getCurrentProject()
  {
    return m_currentProject.get();
  }
}
