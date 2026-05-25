#pragma once

#include <hc/graphics/resource/texture/hcITextureFactory.h>

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
     * @copydoc ITextureFactory::createTexture()
     */
    SharedPtr<ITexture> createTexture() override;

    /**
     * @copydoc ITextureFactory::createTexture(SharedPtr<Image>)
     */
    SharedPtr<ITexture> createTexture(SharedPtr<Image> image) override;
  };
}
