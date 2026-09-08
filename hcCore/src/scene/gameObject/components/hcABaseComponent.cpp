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

    writer.startWritingObject(static_cast<UInt32>(getType()), composedVersion);
    onSerialize(writer);
    writer.finishWritingObject();
  }

  void ABaseComponent::deserialize(io::BinaryReader& reader)
  {
    UInt32 composedVersion = (static_cast<UInt32>(ABASE_COMPONENT_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(composedVersion))
    {
      reader.finishReadingObject();
      return;
    }

    if (!header.matchType(static_cast<UInt32>(getType())))
    {
      reader.finishReadingObject();
      throw RuntimeErrorException(
        String::Format(
          "Component type mismatch during deserialization. Expected type: %u, but got: %u",
          static_cast<UInt32>(getType()),
          header.type
        )
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
