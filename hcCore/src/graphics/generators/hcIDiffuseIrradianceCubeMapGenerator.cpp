#include "hc/graphics/generators/hcIDiffuseIrradianceCubeMapGenerator.h"

namespace hc::graphics::generators
{
  static constexpr UInt32 DIFFUSE_IRRADIANCE_CUBE_MAP_GENERATOR_SETTINGS_VERSION = 1;

  void DiffuseIrradianceCubeMapGeneratorSettings::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, DIFFUSE_IRRADIANCE_CUBE_MAP_GENERATOR_SETTINGS_VERSION);
    writer.writeSizeT(faceSize);
    writer.writeFloat(sampleDelta);
    writer.writeFloat(sourceMipLevel);
    writer.finishWritingObject();
  }

  void DiffuseIrradianceCubeMapGeneratorSettings::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (header.version != DIFFUSE_IRRADIANCE_CUBE_MAP_GENERATOR_SETTINGS_VERSION)
    {
      reader.finishReadingObject();
      return;
    }

    faceSize = reader.readSizeT();
    sampleDelta = reader.readFloat();
    sourceMipLevel = reader.readFloat();
    reader.finishReadingObject();
  }

  void DiffuseIrradianceCubeMapGeneratorSettings::clear()
  {
    faceSize = 32;
    sampleDelta = 0.025f;
    sourceMipLevel = 0.0f;
  }

  IDiffuseIrradianceCubeMapGenerator::~IDiffuseIrradianceCubeMapGenerator() = default;
  IDiffuseIrradianceCubeMapGenerator::IDiffuseIrradianceCubeMapGenerator() = default;
}
