#pragma once

#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  class HC_CORE_EXPORT ICubeMapDescriptorAssetManager :
    public ATypedAssetManager<CubeMapDescriptor>
  {
  public:
    virtual ~ICubeMapDescriptorAssetManager();

  protected:
    ICubeMapDescriptorAssetManager();
  };
}
