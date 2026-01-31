#pragma once

#include <hc/hcIModelLoader.h>
#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  class HC_ASSET_LOADERS_EXPORT ModelLoader : public IModelLoader
  {
  public:
    ModelLoader();
    virtual ~ModelLoader() override;

    /**
     * @copydoc IAssetLoader::load
     */
    SharedPtr<Model> load(const Path& filePath) override;
  };
}
