#include "hc/editor/services/projectManager/hcProjectManager.h"
#include <fstream>
#include "hc/editor/services/projectManager/hcProject.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"
#include "hc/editor/serialization/hcProjectSerializer.h"

namespace hc::editor
{
  ProjectManager::ProjectManager(IAssetManager& assetManager) :
    m_assetManager(assetManager),
    m_isProjectOpen(false),
    m_currentProject(nullptr),
    m_listeners()
  {
  }

  void ProjectManager::prepare()
  {
    m_currentProject = UniquePtr<Project>(Project::CreateEmpty());
  }

  void ProjectManager::destroy()
  {
    closeProject();
    m_listeners.clear();
  }

  bool ProjectManager::openProject(const Path& projectPath)
  {
    try
    {
      closeProject();

      m_currentProject = serialization::ProjectSerializer::Deserialize(projectPath);
      if (!m_currentProject)
        return false;

      m_currentProject->setProjectFilePath(projectPath);
      m_assetManager.setRootPath(projectPath.parent_path());
      m_isProjectOpen = true;

      LogService::Message(
        "Project opened successfully: " + projectPath.string()
      );

      for (auto* listener : m_listeners)
        listener->onProjectOpened();

      return true;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to open project file: " + projectPath.string() + " Error: " + e.what()
      );
      return false;
    }
  }

  bool ProjectManager::saveProject(const Path& savePath)
  {
    if (!m_currentProject)
    {
      LogService::Error("Current project data is invalid. Cannot save.");
      return false;
    }

    try
    {
      if (serialization::ProjectSerializer::Serialize(*m_currentProject, savePath))
      {
        m_currentProject->setProjectFilePath(savePath);
        m_assetManager.setRootPath(savePath.parent_path());

        LogService::Message(
          "Project saved successfully: " + savePath.string()
        );

        return true;
      }
      else
      {
        LogService::Error(
          "Failed to save project file: " + savePath.string()
        );
        return false;
      }
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to save project file: " + savePath.string() + " Error: " + e.what()
      );
      return false;
    }
  }

  bool ProjectManager::closeProject()
  {
    if (m_isProjectOpen && m_currentProject)
    {
      m_currentProject = nullptr;
      m_isProjectOpen = false;
      m_assetManager.setRootPath(Path());

      for (auto* listener : m_listeners)
        listener->onProjectClosed();

      return true;
    }

    return false;
  }

  Path ProjectManager::getCurrentProjectPath() const
  {
    if (m_currentProject)
      return m_currentProject->getProjectFilePath();
    return Path();
  }

  Path ProjectManager::getCurrentProjectDirectory() const
  {
    if (m_currentProject)
      return m_currentProject->getProjectFilePath().parent_path();
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
