#include "hc/hcGameObject.h"
#include <algorithm>

namespace hc
{
  GameObject::GameObject()
    : m_parent(nullptr)
  {
  }

  GameObject::~GameObject()
  {
    if (m_parent)
      m_parent->removeChild(this);
  }

  void GameObject::draw(IGraphicsManager& graphicsManager)
  {
    for (auto& child : m_children)
      child->draw(graphicsManager);
  }

  void GameObject::update(float deltaTime)
  {
    for (auto& child : m_children)
      child->update(deltaTime);
  }

  GameObject* GameObject::getParent() const
  {
    return m_parent;
  }

  void GameObject::addChild(UniquePtr<GameObject> child)
  {
    if (!child)
      return;

    if (child->m_parent)
      child->m_parent->removeChild(child.get());

    child->m_parent = this;
    m_children.push_back(std::move(child));
  }

  UniquePtr<GameObject> GameObject::removeChild(GameObject* child)
  {
    if (!child)
      return nullptr;

    auto it = std::find_if(
      m_children.begin(), m_children.end(),
      [child](const UniquePtr<GameObject>& ptr)
      {
        return ptr.get() == child;
      }
    );

    if (it != m_children.end())
    {
      (*it)->m_parent = nullptr;
      UniquePtr<GameObject> removed = std::move(*it);
      m_children.erase(it);
      return removed;
    }

    return nullptr;
  }

  const Vector<UniquePtr<GameObject>>& GameObject::getChildren() const
  {
    return m_children;
  }

  Matrix4 GameObject::getWorldMatrix() const
  {
    if (m_parent)
      return m_parent->getWorldMatrix() * getMatrix();
    else
      return getMatrix();
  }
}
