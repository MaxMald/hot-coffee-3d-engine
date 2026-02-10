#pragma once

#include <hc/hcITextureFactory.h>

namespace hc
{
  /**
   * @brief Factory for creating OpenGL texture instances.
   */
  class OpenGlTextureFactory : public ITextureFactory
  {
  public:
    OpenGlTextureFactory() = default;
    ~OpenGlTextureFactory() override = default;

    /**
     * @brief Creates an OpenGL texture instance from the given image.
     * 
     * @param image Shared pointer to the image data.
     * 
     * @return Shared pointer to the created ITexture instance.
     */
    SharedPtr<ITexture> createTexture(SharedPtr<Image> image) override;
  };
}
