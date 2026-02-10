#pragma once

#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  class HC_ASSET_LOADERS_EXPORT ImageLoader : public IAssetLoader<Image>
  {
  public:
    ImageLoader() = default;
    virtual ~ImageLoader() = default;

    /**
     * @copydoc IAssetLoader::load
     */
    SharedPtr<Image> load(const Path& filePath) override;
  };
}
