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
    return MakeShared<OpenGlTexture>(image);
  }
}
