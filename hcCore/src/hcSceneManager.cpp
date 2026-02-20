#include "hc/hcSceneManager.h"
#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  SceneManager::SceneManager() :
    m_scenes(),
    m_activeScene(nullptr),
    m_gameObjectFactory(nullptr)
  {
  }

  SceneManager::~SceneManager()
  {
    destroy();
  }

  void SceneManager::initialize(
    UniquePtr<IGameObjectFactory> gameObjectFactory
  )
  {
    if (!gameObjectFactory)
    {
      throw InvalidArgumentException(
        "GameObjectFactory pointer is null."
      );
    }

    m_gameObjectFactory = std::move(gameObjectFactory);
  }

  void SceneManager::draw()
  {
    if (m_activeScene)
      m_activeScene->draw();
  }

  bool SceneManager::removeScene(const String& name)
  {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end())
    {
      if (m_activeScene == it->second.get())
      {
        m_activeScene->deactivate();
        m_activeScene = nullptr;
      }

      it->second->destroy();
      m_scenes.erase(it);
      return true;
    }

    return false;
  }

  bool SceneManager::hasScene(const String& name) const
  {
    return m_scenes.find(name) != m_scenes.end();
  }

  Scene* SceneManager::getScene(const String& name) const
  {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end())
      return it->second.get();

    return nullptr;
  }

  bool SceneManager::setActiveScene(const String& name)
  {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end())
    {
      if (m_activeScene)
        m_activeScene->deactivate();

      m_activeScene = it->second.get();
      m_activeScene->activate();

      return true;
    }

    return false;
  }

  Scene* SceneManager::getActiveScene() const
  {
    return m_activeScene;
  }

  void SceneManager::update(const Time& elapsedTime)
  {
    if (m_activeScene)
      m_activeScene->update(elapsedTime);
  }

  void SceneManager::clear()
  {
    if (m_activeScene)
    {
      m_activeScene->deactivate();
      m_activeScene = nullptr;
    }

    for (auto& pair : m_scenes)
    {
      if (pair.second)
        pair.second->destroy();
    }

    m_scenes.clear();
  }

  void SceneManager::destroy()
  {
    clear();
    m_gameObjectFactory.reset();
  }

  void SceneManager::addScene(const String& name, UniquePtr<Scene> scene)
  {
    if (!m_gameObjectFactory)
    {
      throw RuntimeErrorException(
        "IGameObjectFactory is not initialized. Cannot add scene."
      );
    }

    scene->initialize(m_gameObjectFactory.get());
    m_scenes[name] = std::move(scene);
  }
}
