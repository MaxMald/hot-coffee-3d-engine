#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

#include "hc/editor/services/editorSceneManager/hcIEditorSceneManagerListener.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

using hc::serialization::SceneSerializer;

namespace hc::editor
{
  EditorSceneManager::EditorSceneManager(
    Scene* editorScene,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager,
    ProjectManager& projectManager,
    EditorMetadataManager& editorMetadataManager
  ) :
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_projectManager(projectManager),
    m_editorMetadataManager(editorMetadataManager),
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
    clearScene();
    m_projectManager.unsubscribeListener(this);
    m_listeners.clear();
  }

  bool EditorSceneManager::createNewScene(const Path& scenePath)
  {
    clearScene();
    return saveScene(scenePath);
  }

  bool EditorSceneManager::openScene(const Path& scenePath)
  {
    if (isSceneOpen())
      closeScene();

    if (!scenePath.exists())
    {
      LogService::Error(
        "Failed to open scene: " + scenePath.toString() +
        ". File does not exist."
      );
      return false;
    }

    if (SceneSerializer::Deserialize(
      *m_editorScene,
      scenePath,
      m_assetManager,
      m_graphicsManager
    ))
    {
      m_currentScenePath = scenePath;
      addLastOpenedSceneToMetadata(scenePath);

      LogService::Message("Scene opened successfully: " + scenePath.toString());

      for (auto* listener : m_listeners)
        if (listener)
          listener->onSceneOpened();

      return true;
    }

    clearScene();
    return false;
  }

  bool EditorSceneManager::saveScene(const Path& scenePath)
  {
    assertSceneIsValid();

    if (!scenePath.isCreatable())
    {
      LogService::Error(
        "Failed to save scene: " + scenePath.toString() +
        ". File path is not creatable."
      );
      return false;
    }

    if (SceneSerializer::Serialize(*m_editorScene, scenePath, m_assetManager))
    {
      m_currentScenePath = scenePath;
      addLastOpenedSceneToMetadata(scenePath);

      LogService::Message("Scene saved successfully: " + scenePath.toString());
      return true;
    }

    return false;
  }

  void EditorSceneManager::closeScene()
  {
    clearScene();

    for (auto* listener : m_listeners)
      if (listener)
        listener->onSceneClosed();
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

    const Vector<Path>& recentOpenedScenes = m_editorMetadataManager
      .getProjectMetadataManager()
      .getLastOpenedScenePaths();

    if (recentOpenedScenes.empty())
      return;

    Path lastOpenedScenePath = recentOpenedScenes.front();
    if (lastOpenedScenePath.empty())
      return;

    try
    {
      Path projectDirectory = m_projectManager.getCurrentProjectDirectory();
      Path absoluteScenePath = lastOpenedScenePath.toAbsolute(projectDirectory);

      if (!absoluteScenePath.exists())
        return;

      openScene(absoluteScenePath);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to open last opened scene: '%s' for project '%s'. Error: %s",
          lastOpenedScenePath.toString().c_str(),
          m_projectManager.getCurrentProjectDirectory().toString().c_str(),
          e.what()
        )
      );
    }
  }

  void EditorSceneManager::onProjectClosed()
  {
    if (isSceneOpen())
      closeScene();
  }

  void EditorSceneManager::addLastOpenedSceneToMetadata(const Path& scenePath)
  {
    Project* currentProject = m_projectManager.getCurrentProject();
    if (currentProject == nullptr)
      return;

    m_editorMetadataManager.getProjectMetadataManager().saveLastOpenedScenePath(
      currentProject->getProjectFilePath(),
      scenePath
    );
  }

  void EditorSceneManager::assertSceneIsValid() const
  {
    if (!m_editorScene)
      throw RuntimeErrorException("EditorSceneManager requires a valid Scene pointer");
  }

  void EditorSceneManager::clearScene()
  {
    m_currentScenePath.clear();
    if (m_editorScene != nullptr)
      m_editorScene->clear();
    m_assetManager.clear();
  }
}
