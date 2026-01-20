#pragma once

#include "hc/hcITextureLoader.h"
#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  class HC_ASSET_LOADERS_EXPORT TextureLoader : public ITextureLoader
  {
  public:
    TextureLoader();
    virtual ~TextureLoader() override;

    /**
     * @copydoc ITextureLoader::loadTexture
     */
    virtual SharedPtr<Texture> loadTexture(const Path& filePath) override;
  };
}
