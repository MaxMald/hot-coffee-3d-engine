#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Structure containing settings for window creation.
   */
  struct HC_CORE_EXPORT WindowSettings
  {
    /**
     * @brief The x-coordinate of the window's position on the screen.
     */
    Int32 x = 0;

    /**
     * @brief The y-coordinate of the window's position on the screen.
     */
    Int32 y = 0;

    /**
     * @brief The width of the window in pixels.
     */
    UInt32 width = 800;

    /**
     * @brief The height of the window in pixels.
     */
    UInt32 height = 600;

    /**
     * @brief The title of the window.
     */
    String title = "HotCoffee Application";
  };
}
