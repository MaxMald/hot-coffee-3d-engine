#include "hc/graphics/generators/hcOpenGlEquirectangularToCubeMapGenerator.h"

#include "hc/graphics/resource/texture/hcOpenGlTexture.h"
#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"

namespace hc::graphics::generators
{
  OpenGlEquirectangularToCubeMapGenerator::OpenGlEquirectangularToCubeMapGenerator()
  {

  }

  OpenGlEquirectangularToCubeMapGenerator::~OpenGlEquirectangularToCubeMapGenerator()
  {

  }

  void OpenGlEquirectangularToCubeMapGenerator::generate(
    const ITexture& equirectangularTexture,
    UInt32 cubeMapFaceSize,
    ICubeMap& cubeMap
  )
  {
    if (!equirectangularTexture.isValid())
      throw InvalidArgumentException("Invalid equirectangular texture provided.");
    if (!cubeMap.isValid())
      throw InvalidArgumentException("Invalid cubemap provided.");
    if (cubeMapFaceSize == 0)
      throw InvalidArgumentException("Cubemap face size must be greater than zero.");



    // Implementation for generating a cubemap from an equirectangular texture using OpenGL.
    // This is a placeholder for the actual OpenGL code that would perform the conversion.
  }
}
