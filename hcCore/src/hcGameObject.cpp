#include "hc/hcGameObject.h"
#include <algorithm>
#include "hc/hcIComponent.h"
#include "hc/hcRenderContext.h"
#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  GameObject::GameObject(
    const String& name,
    IGameObjectFactory& gameObjectFactory,
    ComponentFactoriesManager& componentFactoriesManager
  ) :
    m_parent(nullptr),
    m_gameObjectFactory(gameObjectFactory),
    m_componentFactoriesManager(componentFactoriesManager),
    m_name(name)
  {
  }

  GameObject::~GameObject()
  {
    destroy();
  }

  void GameObject::draw(const RenderContext& renderContext)
  {
    RenderContext localRenderContext = renderContext;
    localRenderContext.transform *= getMatrix();

    for (IDrawable* drawableComponent : m_drawableComponents)
    {
      if (drawableComponent)
        drawableComponent->draw(localRenderContext);
    }

    for (auto& child : m_children)
      child->draw(localRenderContext);
  }

  void GameObject::update(const Time& elapsedTime)
  {
    for (auto& child : m_children)
      child->update(elapsedTime);
  }

  void GameObject::setName(const String& name)
  {
    m_name = name;
  }

  const String& GameObject::getName() const
  {
    return m_name;
  }

  GameObject* GameObject::getParent() const
  {
    return m_parent;
  }

  void GameObject::addChild(UniquePtr<GameObject> child)
  {
    if (!child)
    {
      throw InvalidArgumentException(
        "Cannot add a null child GameObject."
      );
    }

    if (child->m_parent)
      child->m_parent->removeChild(child.get());

    child->m_parent = this;
    m_children.push_back(std::move(child));
  }

  GameObject* GameObject::createChild(const String& childName)
  {
    UniquePtr<GameObject> newChild = m_gameObjectFactory.create(childName);
    if (!newChild)
    {
      throw RuntimeErrorException(
        "Failed to create child GameObject with name: " + childName
      );
    }

    GameObject* newChildPtr = newChild.get();
    addChild(std::move(newChild));
    return newChildPtr;
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

  GameObject* GameObject::getChild(const String& name)
  {
    for (const auto& child : m_children)
    {
      if (child->getName() == name)
        return child.get();
    }
    return nullptr;
  }

  Vector<GameObject*> GameObject::getChildrenByName(const String& name)
  {
    Vector<GameObject*> matchingChildren;
    for (const auto& child : m_children)
    {
      if (child->getName() == name)
        matchingChildren.push_back(child.get());
    }
    return matchingChildren;
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

  Vector<IComponent*> GameObject::getComponents() const
  {
    Vector<IComponent*> components;
    components.reserve(m_components.size());

    for (const auto& pair : m_components)
      components.push_back(pair.second.get());
    return components;
  }

  void GameObject::destroy()
  {
    m_components.clear();

    if (m_parent)
      m_parent->removeChild(this);

    for (auto& child : m_children)
      child->m_parent = nullptr;

    m_children.clear();
  }
}
