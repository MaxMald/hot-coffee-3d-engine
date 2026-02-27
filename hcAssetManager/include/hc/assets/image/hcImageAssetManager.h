#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  class ImageAssetManager : public IImageAssetManager
  {
  public:
    ImageAssetManager();
    virtual ~ImageAssetManager();

    SharedPtr<Image> load(const Path& path) override;
    SharedPtr<Image> get(const Path& path) const override;
    bool isLoaded(const Path& path) const override;
    void clear() override;
  };
}
