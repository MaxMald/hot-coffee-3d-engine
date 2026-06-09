#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

#include "hc/editor/services/editorSceneManager/hcIEditorSceneManagerListener.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"

using hc::serialization::SceneSerializer;

namespace hc::editor
{
  EditorSceneManager::EditorSceneManager(
    Scene* editorScene,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager,
    ProjectManager& projectManager
  ) :
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_projectManager(projectManager),
    m_editorScene(editorScene),
    m_currentScenePath(),
    m_listeners()
  {}

  // Set editor scene instead

  void EditorSceneManager::prepare()
  {
    m_projectManager.subscribeListener(this);
  }

  void EditorSceneManager::destroy()
  {
    m_projectManager.unsubscribeListener(this);
    m_listeners.clear();
  }

  bool EditorSceneManager::openScene(const Path& scenePath)
  {
    if (isSceneOpen())
      closeScene();

    if (SceneSerializer::Deserialize(
      *m_editorScene,
      scenePath,
      m_assetManager,
      m_graphicsManager
    ))
    {
      m_currentScenePath = scenePath;
      updateLastOpenedSceneInProject();

      LogService::Message(
        "Scene opened successfully: " + scenePath.string()
      );

      for (auto* listener : m_listeners)
      {
        if (listener)
          listener->onSceneOpened();
      }

      return true;
    }

    return false;
  }

  bool EditorSceneManager::saveScene(const Path& scenePath)
  {
    assertSceneIsValid();
    if (SceneSerializer::Serialize(*m_editorScene, scenePath, m_assetManager))
    {
      m_currentScenePath = scenePath;
      updateLastOpenedSceneInProject();

      LogService::Message(
        "Scene saved successfully: " + scenePath.string()
      );

      return true;
    }
    return false;
  }

  void EditorSceneManager::closeScene()
  {
    if (!isSceneOpen())
      return;

    assertSceneIsValid();

    m_editorScene->clear();
    m_currentScenePath.clear();

    for (auto* listener : m_listeners)
    {
      if (listener)
        listener->onSceneClosed();
    }
  }

  bool EditorSceneManager::isSceneOpen() const
  {
    return !m_currentScenePath.empty();
  }

  const Path& EditorSceneManager::getCurrentScenePath() const
  {
    return m_currentScenePath;
  }

  Scene& EditorSceneManager::getEditorScene()
  {
    if (!m_editorScene)
      throw RuntimeErrorException("Editor scene is undefined or invalid");
    return *m_editorScene;
  }

  void EditorSceneManager::subscribeListener(IEditorSceneManagerListener* listener)
  {
    if (!listener)
      return;

    auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it == m_listeners.end())
      m_listeners.push_back(listener);
  }

  void EditorSceneManager::unsubscribeListener(IEditorSceneManagerListener * listener)
  {
    if (!listener)
      return;

    auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end())
      m_listeners.erase(it);
  }

  void EditorSceneManager::onProjectOpened()
  {
    if (isSceneOpen())
      closeScene();

    Project* currentProject = m_projectManager.getCurrentProject();
    if (!currentProject)
      return;

    String relativeScenePath = currentProject->getPathToLastOpenedScene();
    if (relativeScenePath.empty())
      return;

    const Path lastOpenedScenePath = AssetPath::ToAbsolute(
      relativeScenePath,
      m_projectManager.getCurrentProjectDirectory()
    );

    openScene(lastOpenedScenePath);
  }

  void EditorSceneManager::onProjectClosed()
  {
    if (isSceneOpen())
      closeScene();
  }

  void EditorSceneManager::updateLastOpenedSceneInProject()
  {
    Project* currentProject = m_projectManager.getCurrentProject();
    if (currentProject)
      currentProject->setPathToLastOpenedScene(m_currentScenePath);
  }

  void EditorSceneManager::assertSceneIsValid() const
  {
    if (!m_editorScene)
      throw RuntimeErrorException("EditorSceneManager requires a valid Scene pointer");
  }
}
