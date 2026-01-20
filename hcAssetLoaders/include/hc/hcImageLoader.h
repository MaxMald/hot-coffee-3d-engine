#pragma once

#include "hc/hcIImageLoader.h"
#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  class HC_ASSET_LOADERS_EXPORT ImageLoader : public IImageLoader
  {
  public:
    ImageLoader();
    virtual ~ImageLoader() override;

    /**
     * @copydoc IAssetLoader::load
     */
    SharedPtr<Image> load(const Path& filePath) override;
  };
}
