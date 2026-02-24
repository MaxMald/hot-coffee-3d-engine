#include "hc/scene/hcSceneGraph.h"

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
    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->draw(renderContext);
    }
  }

  void SceneGraph::update(const Time& elapsedTime)
  {
    for (const UniquePtr<GameObject>& root : m_roots)
    {
      if (root)
        root->update(elapsedTime);
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
          "Cannot set GameObject with name '%s' because it already has a parent.",
          root->getName().c_str()
        )
      );
    }

    m_roots.push_back(std::move(root));
  }

  UniquePtr<GameObject> SceneGraph::removeRoot(const String& name)
  {
    auto it = std::find_if(
      m_roots.begin(), m_roots.end(),
      [&name](const UniquePtr<GameObject>& root) 
      {
        return root && root->getName() == name;
      }
    );

    if (it != m_roots.end())
    {
      UniquePtr<GameObject> removedRoot = std::move(*it);
      m_roots.erase(it);
      return removedRoot;
    }

    return nullptr;
  }

  GameObject* SceneGraph::getRoot(const String& name) const
  {
    auto it = std::find_if(
      m_roots.begin(), m_roots.end(),
      [&name](const UniquePtr<GameObject>& root) 
      {
        return root && root->getName() == name;
      }
    );

    if (it != m_roots.end())
      return it->get();

    return nullptr;
  }

  const Vector<UniquePtr<GameObject>>& SceneGraph::getRoots() const
  {
    return m_roots;
  }

  void SceneGraph::clear()
  {
    m_roots.clear();
  }
}
