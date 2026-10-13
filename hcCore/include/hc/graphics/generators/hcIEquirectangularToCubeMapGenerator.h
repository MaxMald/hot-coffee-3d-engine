#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  class ITexture;
  class ICubeMap;

  namespace graphics::generators
  {
    /**
   * @brief Settings for the generation of a cube map from an equirectangular image.
   */
    struct HC_CORE_EXPORT EquirectangularToCubeMapGeneratorSettings : public io::ISerializable
    {
      Vector3f invert = Vector3f(1.0f, 1.0f, 1.0f);  ///< Inversion factors for the X, Y, and Z axes of the cube's vertices.
      UInt32 customFaceSize = 512;                   ///< Custom face size for the generated cube map (used if useCustomFaceSize is true).
      bool useCustomFaceSize = false;                ///< Whether to use a custom face size for the generated cube map.

      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();

      /**
       * @brief Converts the EquirectangularToCubeMapGeneratorSettings to a data block
       * structure for GPU usage.
       * @return dataBlockStructure::CubeMapGenerator The populated data block structure.
       */
      inline dataBlockStructure::CubeMapGenerator getCubeMapGeneratorDataBlockStructure() const
      {
        dataBlockStructure::CubeMapGenerator cmgData;
        cmgData.invert = Vector4f(invert, 1.0f);
        return cmgData;
      }
    };

    /**
     * @brief Interface for generating cubemaps from equirectangular textures.
     */
    class HC_CORE_EXPORT IEquirectangularToCubeMapGenerator
    {
    public:
      virtual ~IEquirectangularToCubeMapGenerator();

      /**
       * @brief Generates a cubemap from the given equirectangular texture.
       *
       * @param equirectangularTexture The input equirectangular texture.
       * @param settings The settings for the generation of the cubemap.
       *
       * @returns A shared pointer to the generated cubemap.
       */
      virtual SharedPtr<ICubeMap> generate(
        const ITexture& equirectangularTexture,
        const EquirectangularToCubeMapGeneratorSettings& settings
      ) = 0;

      /**
       * @brief Destroys the generator and releases any associated resources.
       */
      virtual void destroy() = 0;

    protected:
      IEquirectangularToCubeMapGenerator();
    };
  }
}
