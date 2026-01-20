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
     * @copydoc ITextureLoader::load
     */
    SharedPtr<Texture> load(const Path& filePath) override;
  };
}
