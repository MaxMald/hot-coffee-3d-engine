#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"

using hc::serialization::SceneSerializer;

namespace hc::editor
{
  EditorSceneManager::EditorSceneManager(
    Scene* editorScene,
    ProjectManager& projectManager
  ) :
    m_projectManager(projectManager),
    m_editorScene(editorScene),
    m_currentScenePath()
  {
  }

  void EditorSceneManager::prepare()
  {
    m_projectManager.subscribeListener(this);
  }

  void EditorSceneManager::destroy()
  {
    m_projectManager.unsubscribeListener(this);
  }

  bool EditorSceneManager::openScene(const Path& scenePath)
  {
    if (isSceneOpen())
      closeScene();

    if (SceneSerializer::Deserialize(*m_editorScene, scenePath))
    {
      m_currentScenePath = scenePath;
      return true;
    }

    return false;
  }

  bool EditorSceneManager::saveScene(const Path& scenePath)
  {
    if (!isSceneOpen())
      return false;

    assertSceneIsValid();

    if (SceneSerializer::Serialize(*m_editorScene, scenePath))
    {
      m_currentScenePath = scenePath;
      return true;
    }
  }

  void EditorSceneManager::closeScene()
  {
    if (!isSceneOpen())
      return;

    assertSceneIsValid();

    m_editorScene->clear();
    m_currentScenePath.clear();
  }

  bool EditorSceneManager::isSceneOpen() const
  {
    return !m_currentScenePath.empty();
  }

  const Path& EditorSceneManager::getCurrentScenePath() const
  {
    return m_currentScenePath;
  }

  void EditorSceneManager::onProjectOpened()
  {
    if (isSceneOpen())
      closeScene();
  }

  void EditorSceneManager::onProjectClosed()
  {
    if (isSceneOpen())
      closeScene();
  }

  void EditorSceneManager::assertSceneIsValid() const
  {
    if (!m_editorScene)
      throw RuntimeErrorException("EditorSceneManager requires a valid Scene pointer");
  }
}
