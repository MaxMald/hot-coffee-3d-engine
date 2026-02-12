#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneManager;

  namespace sceneManagerFactory
  {
    UniquePtr<SceneManager> create();
  }
}
