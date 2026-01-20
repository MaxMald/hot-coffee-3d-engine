#pragma once

#include "hc/hcIAssetLoader.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcJson.h"

namespace hc
{
  class UnlitMaterialDescriptor;

  class MaterialDescriptorLoader :
    public IAssetLoader<MaterialDescriptor>
  {
  public:
    MaterialDescriptorLoader();
    virtual ~MaterialDescriptorLoader();

    virtual SharedPtr<MaterialDescriptor> load(const Path& path) override;

  private:
    SharedPtr<UnlitMaterialDescriptor> loadUnlitMaterialDescriptor(const Json&);
  };
}
