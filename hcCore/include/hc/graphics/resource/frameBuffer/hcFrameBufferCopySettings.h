#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc::graphics::resource::frameBuffer
{
  /**
   * @brief Structure representing the settings for copying framebuffer content.
   */
  struct FrameBufferCopySettings
  {
    Int32 sourceX = 0;        ///< The x-coordinate of the lower-left corner of the source rectangle.
    Int32 sourceY = 0;        ///< The y-coordinate of the lower-left corner of the source rectangle.
    UInt32 sourceWidth = 0;   ///< The width of the source rectangle in pixels.
    UInt32 sourceHeight = 0;  ///< The height of the source rectangle in pixels.

    Int32 destX = 0;        ///< The x-coordinate of the lower-left corner of the destination rectangle.
    Int32 destY = 0;        ///< The y-coordinate of the lower-left corner of the destination rectangle.
    UInt32 destWidth = 0;   ///< The width of the destination rectangle in pixels.
    UInt32 destHeight = 0;  ///< The height of the destination rectangle in pixels.

    filterMode::Type filter = filterMode::Nearest;  ///< The filtering mode to use when copying the framebuffer content.
  };
}
