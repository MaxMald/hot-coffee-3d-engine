#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneManager;
  class IGraphicsManager;

  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create(IGraphicsManager& graphicsManager);
  }
}
