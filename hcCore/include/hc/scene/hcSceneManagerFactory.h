#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneManager;
  class IGraphicsManager;
  class IAssetManager;

  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create(
      IGraphicsManager& graphicsManager,
      IAssetManager& assetManager
    );
  }
}
