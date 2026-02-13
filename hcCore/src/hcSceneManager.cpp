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

  Scene* SceneManager::createScene(const String& name)
  {
    if (!m_gameObjectFactory)
    {
      throw RuntimeErrorException(
        "IGameObjectFactory is not initialized. Cannot create scene."
      );
    }

    auto scene = MakeUnique<Scene>(*m_gameObjectFactory);
    Scene* scenePtr = scene.get();
    m_scenes[name] = std::move(scene);
    return scenePtr;
  }

  bool SceneManager::removeScene(const String& name)
  {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end())
    {
      if (m_activeScene == it->second.get())
      {
        m_activeScene->onDeactivate();
        m_activeScene = nullptr;
      }

      it->second->onDestroy();
      m_scenes.erase(it);
      return true;
    }

    return false;
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
        m_activeScene->onDeactivate();

      m_activeScene = it->second.get();
      m_activeScene->onActivate();

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
      m_activeScene->onDeactivate();
      m_activeScene = nullptr;
    }

    for (auto& pair : m_scenes)
    {
      if (pair.second)
        pair.second->onDestroy();
    }

    m_scenes.clear();
  }

  void SceneManager::destroy()
  {
    clear();
    m_gameObjectFactory.reset();
  }
}
