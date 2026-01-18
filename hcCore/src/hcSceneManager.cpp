#include "hc/hcSceneManager.h"

namespace hc
{
  SceneManager::SceneManager()
    : m_activeScene(nullptr)
  {
  }

  SceneManager::~SceneManager()
  {
    m_activeScene = nullptr;
    m_scenes.clear();
  }

  void SceneManager::draw(IGraphicsManager& graphicsManager)
  {
    if (m_activeScene)
      m_activeScene->draw(graphicsManager);
  }

  Scene* SceneManager::createScene(const String& name)
  {
    auto scene = MakeUnique<Scene>();
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

  void SceneManager::update(float deltaTime)
  {
    if (m_activeScene)
      m_activeScene->update(deltaTime);
  }
}
