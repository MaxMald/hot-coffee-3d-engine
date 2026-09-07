#include "hc/scene/gameObject/components/hcABaseComponent.h"

namespace hc
{
  ABaseComponent::ABaseComponent(componentType::Type type) :
    m_gameObject(nullptr),
    m_type(type)
  {
  }

  void ABaseComponent::setGameObject(GameObject* gameObject)
  {
    m_gameObject = gameObject;
    onGameObjectSet();
  }

  GameObject* ABaseComponent::getGameObject() const
  {
    return m_gameObject;
  }

  componentType::Type ABaseComponent::getType() const
  {
    return m_type;
  }

  void ABaseComponent::serialize(io::BinaryWriter& writer) const
  {
    writer.writeUInt16(getType());
  }

  void ABaseComponent::deserialize(io::BinaryReader& reader)
  {
    componentType::Type type = static_cast<componentType::Type>(reader.readUInt16());
    if (type != getType())
    {
      throw RuntimeErrorException(
        "Component type mismatch during deserialization."
      );
    }
  }

  void ABaseComponent::onGameObjectSet()
  {
    // Default implementation does nothing.
  }
}
