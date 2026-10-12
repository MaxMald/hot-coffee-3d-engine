#include "hc/assets/image/hcIImageAssetManager.h"

namespace hc
{
  SharedPtr<Image> IImageAssetManager::getDefaultImage(
    defaultImageType::Type type,
    textureFormatType::Type format,
    colorSpaceType::Type colorSpace
  )
  {
    UInt32 key = (static_cast<UInt32>(type) << 16)
      | (static_cast<UInt32>(format) << 8)
      | static_cast<UInt32>(colorSpace);

    auto it = m_defaultImages.find(key);
    if (it != m_defaultImages.end())
      return it->second;

    Color initColor;
    switch (type)
    {
    case defaultImageType::White:
      initColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
      break;
    case defaultImageType::NormalTangent:
      initColor = Color(0.5f, 0.5f, 1.0f, 1.0f);
      break;
    default:
      initColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
    }

    SharedPtr<Image> defaultImage = MakeShared<Image>(
      1, 1, format, colorSpace, initColor
    );

    m_defaultImages[key] = defaultImage;
    return defaultImage;
  }
}
