#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    /**
    * @brief Checks for OpenGL errors and logs them if any are found.
    */
    void checkAndLogPosibleError();

    /**
     * @brief Converts a draw type to the corresponding OpenGL draw mode.
     */
    UInt32 getOpenGlDrawModeFromDrawType(drawType::Type drawType);

    /**
     * @brief Converts an OpenGL draw mode to the corresponding draw type.
     */
    drawType::Type getDrawTypeFromOpenGlMode(UInt32 glMode);
  }
}
