#include "hc/graphics/generators/hcCubeMapGeneratorSettings.h"

namespace hc::graphics::generators
{
  static constexpr UInt32 CUBEMAP_GENERATOR_SETTINGS_VERSION = 1;

  void CubeMapGeneratorSettings::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), CUBEMAP_GENERATOR_SETTINGS_VERSION);
    writer.writeVector3f(invert);
    writer.writeUInt32(customFaceSize);
    writer.writeBool(useCustomFaceSize);
    writer.finishWritingObject();
  }

  void CubeMapGeneratorSettings::deserialize(io::BinaryReader& reader)
  {
    clear();
    io::ObjectHeader header = reader.startReadingObject();
    UInt32 version = header.version;

    if (version >= 1)
    {
      invert = reader.readVector3f();
      customFaceSize = reader.readUInt32();
      useCustomFaceSize = reader.readBool();
    }

    reader.finishReadingObject();
  }

  void CubeMapGeneratorSettings::clear()
  {
    invert = Vector3f(1.0f, 1.0f, 1.0f);
    customFaceSize = 512;
    useCustomFaceSize = false;
  }
}
