#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ITexture;
  class ICubeMap;

  namespace graphics::generators
  {
    struct CubeMapGeneratorSettings;

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
      virtual SharedPtr<ICubeMap>generate(
        const ITexture& equirectangularTexture,
        const CubeMapGeneratorSettings& settings
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
