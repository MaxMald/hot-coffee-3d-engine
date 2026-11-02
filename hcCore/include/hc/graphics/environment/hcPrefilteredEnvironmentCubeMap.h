#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ICubeMap;
}

namespace hc::graphics::environment
{
  /**
   * @brief Structure representing a prefiltered environment cube map.
   */
  struct PrefilteredEnvironmentCubeMap
  {
    SharedPtr<ICubeMap> cubeMap = nullptr;  ///< Shared pointer to the prefiltered environment cube map.
    UInt32 numMipLevels = 1000;             ///< Number of mip levels in the cube map.
  };
}
