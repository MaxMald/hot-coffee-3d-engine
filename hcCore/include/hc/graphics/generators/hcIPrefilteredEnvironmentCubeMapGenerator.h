#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  class ICubeMap;

  namespace graphics::generators
  {
    /**
     * @brief Settings for the generation of a prefiltered environment cube map.
     */
    struct HC_CORE_EXPORT PrefilteredEnvironmentCubeMapGeneratorSettings : public io::ISerializable
    {
      SizeT faceSize = 128;       ///< The size of each face of the generated cube map. Must be a power of two.
      UInt32 numSamples = 1024;   ///< The number of samples to use for prefiltering. Higher values yield better quality but are more computationally expensive.
      UInt32 maxMipLevels = 5;    ///< The maximum number of mip levels to generate. 0 means to generate all mip levels down to 1x1.

      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();

      inline dataBlockStructure::CubeMapGenerator getCubeMapGeneratorDataBlockStructure() const
      {
        dataBlockStructure::CubeMapGenerator cmgData;
        cmgData.sampleCount = numSamples;
        cmgData.roughness = 0.0f;
        return cmgData;
      }
    };

    /**
     * @brief Interface for generating prefiltered environment cube maps.
     */
    class HC_CORE_EXPORT IPrefilteredEnvironmentCubeMapGenerator
    {
    public:
      virtual ~IPrefilteredEnvironmentCubeMapGenerator();

      /**
       * @brief Generates a prefiltered environment cube map from the input cube map using
       * the specified settings.
       * @param inputCubeMap The input cube map to prefilter.
       * @param settings The settings to use for prefiltering.
       * @return A shared pointer to the generated prefiltered environment cube map.
       */
      virtual SharedPtr<ICubeMap> generate(
        const ICubeMap& inputCubeMap,
        const PrefilteredEnvironmentCubeMapGeneratorSettings& settings
      ) = 0;

      /**
       * @brief Destroys the resources associated with the prefiltered environment cube
       * map generator.
       */
      virtual void destroy() = 0;

    protected:
      IPrefilteredEnvironmentCubeMapGenerator();
    };
  }
}
