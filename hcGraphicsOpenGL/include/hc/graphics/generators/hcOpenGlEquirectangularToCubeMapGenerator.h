#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::generators
{
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlEquirectangularToCubeMapGenerator :
    public IEquirectangularToCubeMapGenerator
  {
  public:
    OpenGlEquirectangularToCubeMapGenerator();
    ~OpenGlEquirectangularToCubeMapGenerator() override;

    /**
     * @copydoc IEquirectangularToCubeMapGenerator::generate
     */
    void generate(
      const ITexture& equirectangularTexture,
      UInt32 cubeMapFaceSize,
      ICubeMap& cubeMap
    ) override;

  private:
  };
}
