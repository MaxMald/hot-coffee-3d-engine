#pragma once

#include "hc/hcIAssetLoader.h"
#include "hc/hcImage.h"

namespace hc
{
  class HC_CORE_EXPORT IImageLoader : public IAssetLoader<Image>
  {
  public:
    virtual ~IImageLoader() override = default;
  };
}
