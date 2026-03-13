#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneManager;

  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create();
  }
}
