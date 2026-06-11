#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ICubeMap;
  class IGraphicsManager;
  class IAssetManager;

  struct HC_CORE_EXPORT CubeMapFactory
  {
    static SharedPtr<ICubeMap> CreateFromDescriptor(
      const Path& cubeMapDescriptorSourcePath,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
  };
}
