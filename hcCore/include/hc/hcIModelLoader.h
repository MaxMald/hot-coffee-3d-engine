#pragma once

#include "hc/hcIAssetLoader.h"
#include "hc/hcModel.h"

namespace hc
{
  class HC_CORE_EXPORT IModelLoader : public IAssetLoader<Model>
  {
  public:
    virtual ~IModelLoader() override = default;
  };
}
