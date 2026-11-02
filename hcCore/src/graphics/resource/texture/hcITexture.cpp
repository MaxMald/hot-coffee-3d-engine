#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  TextureInitializationSettings::TextureInitializationSettings() = default;
  TextureInitializationSettings::TextureInitializationSettings(const Image& image) :
    width(static_cast<UInt16>(image.getWidth())),
    height(static_cast<UInt16>(image.getHeight())),
    mipmapMaxLevel(1000),
    format(image.getFormat()),
    colorSpace(image.getColorSpace()),
    minFilter(filterMode::Linear),
    magFilter(filterMode::Linear),
    mipmapMode(mipmapMode::None),
    wrap(wrapMode::Repeat)
  {
  }
}
