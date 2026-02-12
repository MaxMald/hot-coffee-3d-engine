#include "hc/hcGameObject.h"
#include <algorithm>
#include "hc/hcIComponent.h"
#include "hc/hcRenderContext.h"
#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  GameObject::GameObject(
    IGameObjectFactory& gameObjectFactory,
    ComponentFactoriesManager& componentFactoriesManager,
    const String& name
  ) :
    m_parent(nullptr),
    m_name(name),
    m_gameObjectFactory(gameObjectFactory),
    m_componentFactoriesManager(componentFactoriesManager)
  {
  }

  GameObject::~GameObject()
  {
    m_components.clear();

    if (m_parent)
      m_parent->removeChild(this);
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
        "Child GameObject cannot be null when adding to parent."
      );
    }

    if (child->m_parent)
      child->m_parent->removeChild(child.get());

    child->m_parent = this;
    m_children.push_back(std::move(child));
  }

  GameObject* GameObject::createChild(const String& name)
  {
    UniquePtr<GameObject> child = m_gameObjectFactory.create(name);

    if (!child)
      throw RuntimeErrorException(
        "Failed to create child GameObject with name: " + name
      );

    GameObject* childPtr = child.get();
    addChild(std::move(child));
    return childPtr;
  }

  GameObject* GameObject::getChild(const String& name)
  {
    auto it = std::find_if(
      m_children.begin(), m_children.end(),
      [&name](const UniquePtr<GameObject>& child)
      {
        return child->getName() == name;
      }
    );

    if (it != m_children.end())
      return it->get();

    return nullptr;
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

  Vector<IComponent*> GameObject::getComponents() const
  {
    Vector<IComponent*> components;
    components.reserve(m_components.size());

    for (const auto& pair : m_components)
      components.push_back(pair.second.get());

    return components;
  }

  void GameObject::tryRegisterIDrawableComponent(IComponent* component)
  {
    if (!component)
      return;

    IDrawable* drawableComponent = dynamic_cast<IDrawable*>(component);
    if (drawableComponent)
      m_drawableComponents.push_back(drawableComponent);
  }
}
