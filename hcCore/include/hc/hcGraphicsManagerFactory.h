#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IGraphicsManager;
  class IWindow;
  class AssetManager;
  class PluginManager;

  namespace graphicsManagerFactory
  {
    UniquePtr<IGraphicsManager> Create(
      PluginManager& pluginManager,
      IWindow& window,
      AssetManager& assetManager
    );
  }
}
