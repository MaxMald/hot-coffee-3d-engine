#include "hc/scene/gameObject/hcGameObject.h"

#include <algorithm>

#include "hc/scene/gameObject/components/hcIComponent.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"
#include "hc/graphics/hcRenderContext.h"

namespace hc
{
  constexpr UInt32 GAME_OBJECT_VERSION = 1;

  GameObject::GameObject(
    const String& name,
    IGameObjectFactory& gameObjectFactory,
    ComponentFactoriesManager& componentFactoriesManager
  ) :
    m_uuid(UUID::Generate()),
    m_name(name),
    m_parent(nullptr),
    m_gameObjectFactory(gameObjectFactory),
    m_componentFactoriesManager(componentFactoriesManager),
    m_children(),
    m_components(),
    m_drawableComponents(),
    m_updatableComponents()
  {}

  GameObject::~GameObject()
  {
    destroy();
  }

  void GameObject::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), GAME_OBJECT_VERSION);

    Transform::serialize(writer);
    writer.writeString(m_name);
    writer.writeUUID(m_uuid);

    writer.writeSizeT(m_children.size());
    for (const auto& child : m_children)
      child->serialize(writer);

    writer.writeSizeT(m_components.size());
    for (const auto& pair : m_components)
    {
      const IComponent* component = pair.second.get();
      component->serialize(writer);
    }
    writer.finishWritingObject();
  }

  void GameObject::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(GAME_OBJECT_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    Transform::deserialize(reader);
    m_name = reader.readString();
    m_uuid = reader.readUUID();

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
      io::ObjectHeader componentHeader = reader.peekObjectHeader();
      componentType::Type componentType = static_cast<componentType::Type>(componentHeader.type);

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

    reader.finishReadingObject();
  }

  void GameObject::draw(
    const RenderContext& renderContext,
    Vector<DrawCommand>& outDrawCommands
  ) const
  {
    RenderContext localRenderContext = renderContext;
    localRenderContext.transform *= getMatrix();
    localRenderContext.modelPosition = Vector3f(
      localRenderContext.transform.m03,
      localRenderContext.transform.m13,
      localRenderContext.transform.m23
    );

    for (const auto& pair : m_drawableComponents)
    {
      IDrawable* drawableComponent = pair.second;
      if (drawableComponent)
        drawableComponent->draw(localRenderContext, outDrawCommands);
    }

    for (auto& child : m_children)
      child->draw(localRenderContext, outDrawCommands);
  }

  void GameObject::preUpdate(const Time& elapsedTime)
  {
    for (const auto& pair : m_updatableComponents)
    {
      IUpdatableComponent* updatableComponent = pair.second;
      if (updatableComponent)
        updatableComponent->preUpdate(elapsedTime.toSeconds());
    }

    for (auto& child : m_children)
      child->preUpdate(elapsedTime);
  }

  void GameObject::update(const Time& elapsedTime)
  {
    for (const auto& pair : m_updatableComponents)
    {
      IUpdatableComponent* updatableComponent = pair.second;
      if (updatableComponent)
        updatableComponent->update(elapsedTime.toSeconds());
    }

    for (auto& child : m_children)
      child->update(elapsedTime);
  }

  void GameObject::postUpdate(const Time& elapsedTime)
  {
    for (const auto& pair : m_updatableComponents)
    {
      IUpdatableComponent* updatableComponent = pair.second;
      if (updatableComponent)
        updatableComponent->postUpdate(elapsedTime.toSeconds());
    }

    for (auto& child : m_children)
      child->postUpdate(elapsedTime);
  }

  void GameObject::addChild(UniquePtr<GameObject> child)
  {
    if (child == nullptr)
      throw InvalidArgumentException("Cannot add a null child GameObject.");

    if (child.get() == this)
      throw InvalidArgumentException("Cannot add a GameObject as a child of itself.");

    if (child->m_parent != nullptr)
      throw InvalidArgumentException("The child GameObject already has a parent.");

    child->m_parent = this;
    m_children.push_back(std::move(child));
    updateChildrenIndexMap();
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

    Int32 size = static_cast<Int32>(m_children.size());
    for (Int32 i = size - 1; i >= 0; --i)
    {
      auto& childPtr = m_children[i];
      if (childPtr.get() == child)
      {
        UniquePtr<GameObject> removedChild = std::move(childPtr);
        removedChild->m_parent = nullptr;

        m_children.erase(m_children.begin() + i);
        updateChildrenIndexMap();
        return removedChild;
      }
    }

    return nullptr;
  }

  UniquePtr<GameObject> GameObject::removeChild(const UUID& uuid)
  {
    auto it = m_childrenIndexMap.find(uuid);
    if (it != m_childrenIndexMap.end())
    {
      Int32 index = static_cast<Int32>(it->second);
      auto& childPtr = m_children[index];
      UniquePtr<GameObject> removedChild = std::move(childPtr);
      removedChild->m_parent = nullptr;

      m_children.erase(m_children.begin() + index);
      updateChildrenIndexMap();
      return removedChild;
    }
    return nullptr;
  }

  UniquePtr<GameObject> GameObject::removeDescendant(const UUID& uuid)
  {
    UniquePtr<GameObject> removedChild = removeChild(uuid);
    if (removedChild != nullptr)
      return removedChild;

    for (auto& child : m_children)
    {
      UniquePtr<GameObject> descendant = child->removeDescendant(uuid);
      if (descendant != nullptr)
        return descendant;
    }
    return nullptr;
  }

  GameObject* GameObject::getDescendant(const String& name) const
  {
    for (const auto& child : m_children)
    {
      if (child->getName() == name)
        return child.get();

      GameObject* descendant = child->getDescendant(name);
      if (descendant)
        return descendant;
    }
    return nullptr;
  }

  GameObject* GameObject::getDescendant(const UUID& uuid) const
  {
    GameObject* child = getChild(uuid);
    if (child)
      return child;

    for (const auto& child : m_children)
    {
      GameObject* descendant = child->getDescendant(uuid);
      if (descendant)
        return descendant;
    }

    return nullptr;
  }

  Vector<GameObject*> GameObject::getChildrenByName(const String& name) const
  {
    Vector<GameObject*> matchingChildren;
    for (const auto& child : m_children)
    {
      if (child->getName() == name)
        matchingChildren.push_back(child.get());
    }
    return matchingChildren;
  }

  void GameObject::getDescendants(Vector<GameObject*>& outDescendants) const
  {
    for (const auto& child : m_children)
    {
      outDescendants.push_back(child.get());
      child->getDescendants(outDescendants);
    }
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
    return Matrix4::ExtractTranslation(worldMatrix);
  }

  Matrix4 GameObject::getWorldRotationMatrix() const
  {
    if (m_parent)
      return m_parent->getWorldRotationMatrix() * Matrix4::Rotation(getRotation());
    else
      return Matrix4::Rotation(getRotation());
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

  bool GameObject::removeComponent(IComponent* component)
  {
    if (!component)
      return false;

    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
      if (it->second.get() == component)
      {
        UUID componentUUID = component->getUUID();
        m_drawableComponents.erase(componentUUID);
        m_updatableComponents.erase(componentUUID);
        m_components.erase(it);
        return true;
      }
    }

    return false;
  }

  void GameObject::clear()
  {
    m_components.clear();
    m_drawableComponents.clear();
    m_updatableComponents.clear();

    for (auto& child : m_children)
    {
      child->m_parent = nullptr;
      child->destroy();
    }
      
    m_children.clear();
    m_childrenIndexMap.clear();
  }

  void GameObject::addComponent(UniquePtr<IComponent> component)
  {
    if (!component)
      throw InvalidArgumentException("Cannot add a null component.");

    TypeIndex typeIndex(typeid(*component));
    IComponent* componentPtr = component.get();
    UUID componentUUID = componentPtr->getUUID();

    m_components[typeIndex] = std::move(component);

    if (auto* drawable = dynamic_cast<IDrawable*>(componentPtr))
      m_drawableComponents[componentUUID] = drawable;

    if (auto* updatable = dynamic_cast<IUpdatableComponent*>(componentPtr))
      m_updatableComponents[componentUUID] = updatable;

    componentPtr->setGameObject(this);
  }

  void GameObject::destroy()
  {
    clear();

    if (m_parent)
      m_parent->removeChild(this);
  }
}
