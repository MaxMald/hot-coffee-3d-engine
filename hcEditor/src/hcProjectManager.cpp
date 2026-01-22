#include "hc/editor/hcProjectManager.h"

#include "hc/editor/hcProject.h"
#include "hc/editor/hcIProjectManagerListener.h"

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

    m_currentProject = MakeUnique<Project>();
    if (m_currentProject->loadFromFile(projectPath))
    {
      m_currentProjectPath = projectPath;
      m_isProjectOpen = true;

      for (auto* listener : m_listeners)
        listener->onProjectOpened();

      return true;
    }

    m_currentProject = nullptr;
    return false;
  }

  bool ProjectManager::closeProject()
  {
    if (m_isProjectOpen && m_currentProject)
    {
      m_currentProject = nullptr;
      m_isProjectOpen = false;
      m_currentProjectPath.clear();

      for (auto* listener : m_listeners)
        listener->onProjectClosed();

      return true;
    }

    return false;
  }

  Path ProjectManager::getCurrentProjectPath() const
  {
    return m_currentProjectPath;
  }

  Path ProjectManager::getCurrentProjectDirectory() const
  {
    if (m_isProjectOpen)
      return m_currentProjectPath.parent_path();
    return Path();
  }

  bool ProjectManager::isProjectOpen() const
  {
    return m_isProjectOpen;
  }

  Project* ProjectManager::getCurrentProject()
  {
    return m_currentProject.get();
  }

  void ProjectManager::subscribeListener(IProjectManagerListener* listener)
  {
    auto item = std::find(
      m_listeners.begin(),
      m_listeners.end(),
      listener
    );

    if (item == m_listeners.end())
      m_listeners.push_back(listener);
  }

  void ProjectManager::unsubscribeListener(IProjectManagerListener* listener)
  {
    auto item = std::find(
      m_listeners.begin(),
      m_listeners.end(),
      listener
    );

    if (item != m_listeners.end())
      m_listeners.erase(item);
  }
}
