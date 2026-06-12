#include "hc/graphics/resource/texture/hcOpenGlTextureFactory.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
  SharedPtr<ITexture> OpenGlTextureFactory::createTexture()
  {
    return MakeShared<OpenGlTexture>();
  }

  SharedPtr<ITexture> OpenGlTextureFactory::createTexture(
    SharedPtr<Image> image
  )
  {
    if (!image)
      throw InvalidArgumentException("Image pointer cannot be null when creating a texture.");

    SharedPtr<OpenGlTexture> texture = MakeShared<OpenGlTexture>();
    texture->initialize(*image);
    return texture;
  }
}
