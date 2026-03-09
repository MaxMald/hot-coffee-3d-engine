#pragma once

#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  /**
   * @brief Interface for managing image assets.
   */
  class IImageAssetManager : public ATypedAssetManager<Image>
  {
  public:
    virtual ~IImageAssetManager() = default;

  protected:
    IImageAssetManager() = default;
  };
}
