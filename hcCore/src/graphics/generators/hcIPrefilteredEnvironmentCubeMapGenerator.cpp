#include "hc/graphics/generators/hcIPrefilteredEnvironmentCubeMapGenerator.h"

#include "hc/graphics/resource/cubeMap/hcICubeMap.h"

namespace hc
{
  namespace graphics::generators
  {
    static constexpr UInt32 PREFILTERED_ENVIRONMENT_CUBE_MAP_GENERATOR_SETTINGS_VERSION = 1;

    void PrefilteredEnvironmentCubeMapGeneratorSettings::serialize(
      io::BinaryWriter& writer
    ) const
    {
      writer.startWritingObject(0, PREFILTERED_ENVIRONMENT_CUBE_MAP_GENERATOR_SETTINGS_VERSION);
      writer.writeUInt32(faceSize);
      writer.writeUInt32(numSamples);
      writer.writeUInt32(maxMipLevels);
      writer.finishWritingObject();
    }

    void PrefilteredEnvironmentCubeMapGeneratorSettings::deserialize(
      io::BinaryReader& reader
    )
    {
      clear();

      io::ObjectHeader header = reader.startReadingObject();
      faceSize = reader.readUInt32();
      numSamples = reader.readUInt32();
      maxMipLevels = reader.readUInt32();
      reader.finishReadingObject();
    }

    void PrefilteredEnvironmentCubeMapGeneratorSettings::clear()
    {
      faceSize = 128;
      numSamples = 1024;
      maxMipLevels = 5;
    }

    IPrefilteredEnvironmentCubeMapGenerator::~IPrefilteredEnvironmentCubeMapGenerator() = default;
    IPrefilteredEnvironmentCubeMapGenerator::IPrefilteredEnvironmentCubeMapGenerator() = default;
  }
}
