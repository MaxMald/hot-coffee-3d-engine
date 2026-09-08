#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/utilities/hcTextureFormatType.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  class Image;
  class IAssetManager;

  class HC_CORE_EXPORT ICubeMap : public NonCopyable
  {
  public:
    virtual ~ICubeMap();

    virtual void initialize(
      SharedPtr<CubeMapDescriptor> cubeMapDescriptor,
      IAssetManager& assetManager
    ) = 0;

    virtual bool isValid() const = 0;

    virtual UInt32 getFaceWidth() const = 0;

    virtual UInt32 getFaceHeight() const = 0;

    virtual SharedPtr<CubeMapDescriptor> getCubeMapDescriptor() const = 0;

    virtual void destroy() = 0;

  protected:
    ICubeMap();
  };
}
