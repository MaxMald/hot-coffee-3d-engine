#include "hc/scene/gameObject/components/hcABaseComponent.h"

namespace hc
{
  static constexpr UInt16 ABASE_COMPONENT_VERSION = 1;

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
    UInt32 composedVersion = (static_cast<UInt32>(ABASE_COMPONENT_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());

    writer.startWritingObject(composedVersion);
    writer.writeUInt16(getType());
    onSerialize(writer);
    writer.finishWritingObject();
  }

  void ABaseComponent::deserialize(io::BinaryReader& reader)
  {
    UInt32 composedVersion = (static_cast<UInt32>(ABASE_COMPONENT_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(composedVersion))
    {
      reader.finishReadingObject();
      return;
    }

    componentType::Type type = static_cast<componentType::Type>(reader.readUInt16());
    if (type != getType())
    {
      throw RuntimeErrorException(
        "Component type mismatch during deserialization."
      );
    }

    onDeserialize(reader);
    reader.finishReadingObject();
  }

  void ABaseComponent::onGameObjectSet()
  {
    // Default implementation does nothing.
  }
}
