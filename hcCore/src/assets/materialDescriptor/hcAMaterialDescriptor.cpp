#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 MATERIAL_DESCRIPTOR_VERSION = 1;

  void AMaterialDescriptor::serialize(io::BinaryWriter& writer) const
  {
    UInt32 composedVersion = (static_cast<UInt32>(MATERIAL_DESCRIPTOR_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    writer.startWritingObject(static_cast<UInt32>(getType()), composedVersion);
    writer.writeString(m_name);
    writer.writeUInt8(static_cast<UInt8>(m_renderMode));
    writer.writeFloat(m_alphaCutoutThreshold);
    writer.writeBool(m_doubleSided);
    onSerialization(writer);
    writer.finishWritingObject();
  }

  void AMaterialDescriptor::deserialize(io::BinaryReader& reader)
  {
    clear();

    UInt32 composedVersion = (static_cast<UInt32>(MATERIAL_DESCRIPTOR_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(composedVersion))
    {
      reader.finishReadingObject();
      return;
    }

    if (header.type != static_cast<UInt32>(getType()))
    {
      reader.finishReadingObject();
      throw RuntimeErrorException(
        String::Format(
          "AMaterialDescriptor::deserialize: Type mismatch. Expected type: %u, but got: %u",
          static_cast<UInt32>(getType()),
          header.type
        )
      );
    }

    m_name = reader.readString();
    m_renderMode = static_cast<materialRenderMode::Type>(reader.readUInt8());
    m_alphaCutoutThreshold = reader.readFloat();
    m_doubleSided = reader.readBool();
    onDeserialization(reader);
    reader.finishReadingObject();
  }

  void AMaterialDescriptor::clear()
  {
    m_name.clear();
    m_renderMode = materialRenderMode::Type::Opaque;
    m_alphaCutoutThreshold = 0.5f;
    m_doubleSided = false;
  }

  materialRenderMode::Type AMaterialDescriptor::getRenderMode() const
  {
    return m_renderMode;
  }

  void AMaterialDescriptor::setRenderMode(materialRenderMode::Type renderMode)
  {
    m_renderMode = renderMode;
  }

  void AMaterialDescriptor::setAlphaCutoutThreshold(float threshold)
  {
    m_alphaCutoutThreshold = threshold;
  }

  float AMaterialDescriptor::getAlphaCutoutThreshold() const
  {
    return m_alphaCutoutThreshold;
  }

  bool AMaterialDescriptor::isDoubleSided() const
  {
    return m_doubleSided;
  }

  void AMaterialDescriptor::setDoubleSided(bool doubleSided)
  {
    m_doubleSided = doubleSided;
  }

  AMaterialDescriptor::AMaterialDescriptor(
    const Path& path,
    const String& name,
    materialRenderMode::Type renderMode
  ) :
    Asset(path),
    m_name(name),
    m_renderMode(renderMode),
    m_alphaCutoutThreshold(0.5f),
    m_doubleSided(false)
  {
  }
}
