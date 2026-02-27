#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  class MaterialDescriptorAssetManager : public IMaterialDescriptorAssetManager
  {
  public:
    MaterialDescriptorAssetManager();
    virtual ~MaterialDescriptorAssetManager();

    SharedPtr<AMaterialDescriptor> load(const Path& path) override;
    SharedPtr<AMaterialDescriptor> get(const Path& path) const override;
    bool isLoaded(const Path& path) const override;
    void clear() override;
    SharedPtr<AMaterialDescriptor> getDefault() const override;
  };
}
