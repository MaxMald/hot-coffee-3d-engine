#include "hc/hcOpenGlTextureFactory.h"
#include "hc/hcOpenGlTexture.h"

namespace hc
{
  SharedPtr<ITexture> OpenGlTextureFactory::createTexture(
    SharedPtr<Image> image
  )
  {
    return MakeShared<OpenGlTexture>(image);
  }
}
