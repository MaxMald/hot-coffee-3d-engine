#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/gameObjectSelection/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

namespace hc::editor
{
  GameObjectSelectionService::GameObjectSelectionService(
    EditorSceneManager& editorSceneManager
  ) :
    m_selectedGameObjects(),
    m_listeners(),
    m_editorSceneManager(editorSceneManager)
  {
  }

  void GameObjectSelectionService::prepare()
  {
    m_editorSceneManager.subscribeListener(this);
  }

  void GameObjectSelectionService::destroy()
  {
    m_editorSceneManager.unsubscribeListener(this);
    clearSelection();
  }

  void GameObjectSelectionService::onSceneOpened()
  {
    clearSelection();
  }

  void GameObjectSelectionService::onSceneClosed()
  {
    clearSelection();
  }

  void GameObjectSelectionService::onSceneCleared()
  {
    clearSelection();
  }

  GameObject* GameObjectSelectionService::getFirstSelectedGameObject() const
  {
    if (m_selectedGameObjects.empty())
      return nullptr;
    return m_selectedGameObjects.front();
  }

  const Vector<GameObject*>& GameObjectSelectionService::getSelectedGameObjects() const
  {
    return m_selectedGameObjects;
  }

  void GameObjectSelectionService::selectGameObject(GameObject* gameObject)
  {
    auto item = std::find(
      m_selectedGameObjects.begin(),
      m_selectedGameObjects.end(),
      gameObject);

    if (item == m_selectedGameObjects.end())
    {
      m_selectedGameObjects.push_back(gameObject);

      for (auto* listener : m_listeners)
        listener->onGameObjectSelected(gameObject);
    }
  }

  void GameObjectSelectionService::deselectGameObject(GameObject* gameObject)
  {
    auto item = std::find(
      m_selectedGameObjects.begin(),
      m_selectedGameObjects.end(),
      gameObject
    );

    if (item != m_selectedGameObjects.end())
    {
      m_selectedGameObjects.erase(item);

      for (auto* listener : m_listeners)
        listener->onGameObjectDeselected(gameObject);
    }
  }

  void GameObjectSelectionService::clearSelection()
  {
    for (auto* gameObject : m_selectedGameObjects)
    {
      for (auto* listener : m_listeners)
        listener->onGameObjectDeselected(gameObject);
    }

    m_selectedGameObjects.clear();
  }

  bool GameObjectSelectionService::isGameObjectSelected(GameObject* gameObject) const
  {
    auto item = std::find(
      m_selectedGameObjects.begin(),
      m_selectedGameObjects.end(),
      gameObject
    );

    return item != m_selectedGameObjects.end();
  }

  bool GameObjectSelectionService::hasSelectedGameObjects() const
  {
    return !m_selectedGameObjects.empty();
  }

  void GameObjectSelectionService::subscribe(
    IGameObjectSelectionServiceListener* listener
  )
  {
    auto item = std::find(
      m_listeners.begin(),
      m_listeners.end(),
      listener
    );

    if (item == m_listeners.end())
      m_listeners.push_back(listener);
  }

  void GameObjectSelectionService::unsubscribe(
    IGameObjectSelectionServiceListener* listener
  )
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
