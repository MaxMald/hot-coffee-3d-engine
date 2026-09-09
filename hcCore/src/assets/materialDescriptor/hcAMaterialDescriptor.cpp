#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 MATERIAL_DESCRIPTOR_VERSION = 1;

  void AMaterialDescriptor::serialize(io::BinaryWriter& writer) const
  {
    UInt32 composedVersion = (static_cast<UInt32>(MATERIAL_DESCRIPTOR_VERSION) << 16) | static_cast<UInt32>(getDerivedVersion());
    writer.startWritingObject(static_cast<UInt32>(getType()), composedVersion);
    writer.writeString(name);
    writer.writeUInt8(static_cast<UInt8>(renderMode));
    writer.writeFloat(alphaCutoutThreshold);
    writer.writeBool(doubleSided);
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

    name = reader.readString();
    renderMode = static_cast<materialRenderMode::Type>(reader.readUInt8());
    alphaCutoutThreshold = reader.readFloat();
    doubleSided = reader.readBool();
    onDeserialization(reader);
    reader.finishReadingObject();
  }

  void AMaterialDescriptor::clear()
  {
    name.clear();
    renderMode = materialRenderMode::Type::Opaque;
    alphaCutoutThreshold = 0.5f;
    doubleSided = false;
  }

  AMaterialDescriptor::AMaterialDescriptor(const Path& path) :
    Asset(path),
    name(""),
    renderMode(materialRenderMode::Type::Opaque),
    alphaCutoutThreshold(0.5f),
    doubleSided(false)
  {}
}
