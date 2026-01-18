#include "hc/hcSceneGraph.h"

namespace hc
{
  SceneGraph::SceneGraph()
  {
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::draw(IGraphicsManager& graphicsManager)
  {
    for (const auto& pair : m_roots)
    {
      if (pair.second)
        pair.second->draw(graphicsManager);
    }
  }

  void SceneGraph::update(float deltaTime)
  {
    for (const auto& pair : m_roots)
    {
      if (pair.second)
        pair.second->update(deltaTime);
    }
  }

  void SceneGraph::setRoot(const String& key, UniquePtr<GameObject> root)
  {
    if (root->getParent())
    {
      throw RuntimeErrorException(
        String::Format(
          "Cannot set root GameObject for key '%s' because it already has a parent.",
          key.c_str()
        )
      );
    }

    if (m_roots.find(key) != m_roots.end())
    {
      throw RuntimeErrorException(
        String::Format(
          "Root GameObject for key '%s' already exists. Remove it before setting a new one.",
          key.c_str()
        )
      );
    }

    m_roots[key] = std::move(root);
  }

  UniquePtr<GameObject> SceneGraph::removeRoot(const String& key)
  {
    auto it = m_roots.find(key);
    if (it != m_roots.end())
    {
      UniquePtr<GameObject> removed = std::move(it->second);
      m_roots.erase(it);

      return removed;
    }

    return nullptr;
  }

  GameObject* SceneGraph::getRoot(const String& key) const
  {
    auto it = m_roots.find(key);
    if (it != m_roots.end() && it->second)
      return it->second.get();
    return nullptr;
  }

  const UnorderedMap<String, UniquePtr<GameObject>>& SceneGraph::getRoots() const
  {
    return m_roots;
  }
}
