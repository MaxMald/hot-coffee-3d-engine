#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class HC_CORE_EXPORT ILightShadowTexturesManager
  {
  public:
    virtual ~ILightShadowTexturesManager() = default;

    /**
     * @brief Initializes the light shadow textures manager. This method sets up the necessary
     * resources and prepares the manager for use.
     */
    virtual void initialize() = 0;
    /**
     * @brief Clears the light shadow textures. This method resets the light shadow
     * textures to their default state, effectively removing any previously stored shadow
     * textures.
     */
    virtual void clearLightShadowTextures() = 0;

  protected:
  };
}
