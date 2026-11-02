#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  class ICubeMap;

  namespace graphics::generators
  {
    /**
     * @brief Settings for generating a diffuse irradiance cube map from an existing cube
     * map.
     */
    struct HC_CORE_EXPORT DiffuseIrradianceCubeMapGeneratorSettings : public io::ISerializable
    {
      static constexpr float MINIMUM_SAMPLE_DELTA = 0.005f;           ///< The minimum allowed value for sampleDelta.
      static constexpr float MAXIMUM_SAMPLE_DELTA = Math::Pi * 0.5f;  ///< The maximum allowed value for sampleDelta.

      SizeT faceSize = 32;          ///< The size of each face of the generated cube map. Must be a power of two.
      float sampleDelta = 0.025f;   ///< Smaller values result in higher quality but take longer to compute.
      float sourceMipLevel = 0.0f;  ///< The mip level of the source cube map to sample from. Default is 0 (highest resolution).
      
      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();

      /**
       * @brief Converts the DiffuseIrradianceCubeMapGeneratorSettings to a data block
       * structure for GPU usage.
       * @return dataBlockStructure::CubeMapGenerator The populated data block structure.
       */
      inline dataBlockStructure::CubeMapGenerator getCubeMapGeneratorDataBlockStructure() const
      {
        dataBlockStructure::CubeMapGenerator cmgData;
        cmgData.sampleDelta = sampleDelta;
        cmgData.sourceMipLevel = sourceMipLevel;
        return cmgData;
      }
    };

    /**
     * @brief Interface for generating diffuse irradiance cube maps from existing cube
     * maps.
     */
    class HC_CORE_EXPORT IDiffuseIrradianceCubeMapGenerator
    {
    public:
      virtual ~IDiffuseIrradianceCubeMapGenerator();

      /**
       * @brief Generates a diffuse irradiance cube map from the given input cube map.
       *
       * @param inputCubeMap The input cube map to generate the diffuse irradiance cube
       * map from.
       * @param settings The settings for the generation of the diffuse irradiance cube
       * map.
       *
       * @returns A shared pointer to the generated diffuse irradiance cube map.
       */
      virtual SharedPtr<ICubeMap> generate(
        const ICubeMap& inputCubeMap,
        const DiffuseIrradianceCubeMapGeneratorSettings& settings
      ) = 0;

      /**
       * @brief Destroys the generator and releases any associated resources.
       */
      virtual void destroy() = 0;

    protected:
      IDiffuseIrradianceCubeMapGenerator();
    };
  }
}
