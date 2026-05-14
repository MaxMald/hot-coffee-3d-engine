#include "hc/scene/gameObject/hcGameObject.h"
#include <algorithm>
#include "hc/scene/gameObject/components/hcIComponent.h"
#include "hc/graphics/hcRenderContext.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"

namespace hc
{
  GameObject::GameObject(
    const String& name,
    IGameObjectFactory& gameObjectFactory,
    ComponentFactoriesManager& componentFactoriesManager
  ) :
    m_name(name),
    m_parent(nullptr),
    m_gameObjectFactory(gameObjectFactory),
    m_componentFactoriesManager(componentFactoriesManager),
    m_children(),
    m_components(),
    m_drawableComponents(),
    m_updatableComponents()
  {
  }

  GameObject::~GameObject()
  {
    destroy();
  }

  void GameObject::serialize(BinaryWriter& writer) const
  {
    Transform::serialize(writer);
    writer.writeString(m_name);

    writer.writeSizeT(m_children.size());
    for (const auto& child : m_children)
      child->serialize(writer);

    writer.writeSizeT(m_components.size());
    for (const auto& pair : m_components)
    {
      const IComponent* component = pair.second.get();
      component->serialize(writer);
    }
  }

  void GameObject::deserialize(BinaryReader& reader)
  {
    Transform::deserialize(reader);
    m_name = reader.readString();   

    SizeT childCount = reader.readSizeT();
    for (SizeT i = 0; i < childCount; ++i)
    {
      UniquePtr<GameObject> child = m_gameObjectFactory.create("_toDeserialize");
      child->deserialize(reader);
      addChild(std::move(child));
    }

    SizeT componentCount = reader.readSizeT();
    for (SizeT i = 0; i < componentCount; ++i)
    {
      componentType::Type componentType = static_cast<componentType::Type>(
        reader.peekUInt16()
      );

      UniquePtr<IComponent> component = m_componentFactoriesManager
        .createComponent(componentType);

      if (!component)
      {
        throw RuntimeErrorException(
          "Component deserialization is not implemented. Component type: " +
          componentType::ToString(componentType)
        );
      }

      component->deserialize(reader);
      addComponent(std::move(component));
    }
  }

  void GameObject::draw(const RenderContext& renderContext)
  {
    RenderContext localRenderContext = renderContext;
    localRenderContext.transform *= getMatrix();
    localRenderContext.modelPosition = Vector3f(
      localRenderContext.transform.m03,
      localRenderContext.transform.m13,
      localRenderContext.transform.m23
    );

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
    for (IUpdatableComponent* updatableComponent : m_updatableComponents)
    {
      if (updatableComponent)
        updatableComponent->preUpdate(elapsedTime.toSeconds());
    }

    for (IUpdatableComponent* updatableComponent : m_updatableComponents)
    {
      if (updatableComponent)
        updatableComponent->update(elapsedTime.toSeconds());
    }

    for (IUpdatableComponent* updatableComponent : m_updatableComponents)
    {
      if (updatableComponent)
        updatableComponent->postUpdate(elapsedTime.toSeconds());
    }

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

  Vector3f GameObject::getWorldPosition() const
  {
    Matrix4 worldMatrix = getWorldMatrix();
    return Vector3f(worldMatrix.m03, worldMatrix.m13, worldMatrix.m23);
  }

  Vector3f GameObject::getWorldRotation() const
  {
    if (m_parent)
      return m_parent->getWorldRotation() + getRotation();
    else
      return getRotation();
  }

  Vector<IComponent*> GameObject::getComponents() const
  {
    Vector<IComponent*> components;
    components.reserve(m_components.size());

    for (const auto& pair : m_components)
      components.push_back(pair.second.get());
    return components;
  }

  void GameObject::getComponents(Vector<IComponent*>& outComponents) const
  {
    outComponents.clear();
    outComponents.reserve(m_components.size());

    for (const auto& pair : m_components)
      outComponents.push_back(pair.second.get());
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
