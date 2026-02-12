#include "hc/hcSceneGraph.h"

namespace hc
{
  SceneGraph::SceneGraph()
  {
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::draw(const RenderContext& renderContext)
  {
    for (const auto& root : m_roots)
    {
      if (root)
        root->draw(renderContext);
    }
  }

  void SceneGraph::update(const Time& deltaTime)
  {
    for (const auto& root : m_roots)
    {
      if (root)
        root->update(deltaTime);
    }
  }

  void SceneGraph::addRoot(UniquePtr<GameObject> root)
  {
    if (!root)
    {
      throw RuntimeErrorException(
        "Cannot add a null root GameObject to the SceneGraph."
      );
    }

    if (root->getParent())
    {
      throw RuntimeErrorException(
        String::Format(
          "Cannot set root GameObject for key '%s' because it already has a parent.",
          root->getName().c_str()
        )
      );
    }

    m_roots.push_back(std::move(root));
  }

  UniquePtr<GameObject> SceneGraph::removeRoot(const String& name)
  {
    for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
    {
      if (*it && (*it)->getName() == name)
      {
        UniquePtr<GameObject> removedRoot = std::move(*it);
        m_roots.erase(it);
        return removedRoot;
      }
    }
    return nullptr;
  }

  GameObject* SceneGraph::getRoot(const String& name) const
  {
    for (const auto& root : m_roots)
        if (root && root->getName() == name)
          return root.get();
  
    return nullptr;
  }

  const Vector<UniquePtr<GameObject>>& SceneGraph::getRoots() const
  {
    return m_roots;
  }
}
