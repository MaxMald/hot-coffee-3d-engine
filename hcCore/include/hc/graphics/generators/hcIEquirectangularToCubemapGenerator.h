#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ITexture;
  class ICubeMap;

  namespace graphics::generators
  {
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
       * @param cubeMapFaceSize The size of each face of the cubemap in pixels.
       * @param cubeMap The output cubemap to be generated.
       */
      virtual void generate(
        const ITexture& equirectangularTexture,
        UInt32 cubeMapFaceSize,
        ICubeMap& cubeMap
      ) = 0;

    protected:
      IEquirectangularToCubeMapGenerator();
    };
  }
}
