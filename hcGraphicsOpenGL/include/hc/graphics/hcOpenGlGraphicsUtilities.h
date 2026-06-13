#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    /**
     * @brief Asserts that there are no OpenGL errors.
     */
    void AssertOpenGlHasNoError();

    /**
    * @brief Checks for OpenGL errors and logs them if any are found.
    */
    void CheckAndLogPossibleError();

    /**
     * @brief Converts a draw type to the corresponding OpenGL draw mode.
     */
    UInt32 GetOpenGlDrawModeFromDrawType(drawType::Type drawType);

    /**
     * @brief Converts an OpenGL draw mode to the corresponding draw type.
     */
    drawType::Type GetDrawTypeFromOpenGlMode(UInt32 glMode);

    /**
     * @brief Converts a polygon fill type to the corresponding OpenGL polygon mode.
     */
    UInt32 GetOpenGlPolygonModeFromPolygonFillType(polygonFillType::Type fillType);
    
    /**
     * @brief Converts an OpenGL polygon mode to the corresponding polygon fill type.
     */
    polygonFillType::Type GetPolygonFillTypeFromOpenGlPolygonMode(UInt32 glPolygonMode);

    /**
     * @brief Gets the OpenGL internal format from a color format type and color space
     * type.
     */
    UInt32 GetOpenGLInternalFormatFromColorFormatAndColorSpaceType(
      colorFormatType::Type colorFormat,
      colorSpaceType::Type spaceColor
    );

    /**
     * @brief Converts a color format type to the corresponding OpenGL format.
     */
    UInt32 GetOpenGlFormatFromColorFormatType(colorFormatType::Type colorFormat);
    
    /**
     * @brief Gets the OpenGL data type from a color format type.
     */
    UInt32 GetOpenGLDataTypeFromColorFormatType(colorFormatType::Type colorFormat);

    /**
     * @brief Gets the number of color channels from an OpenGL format.
     */
    UInt8 GetChannelCountFromOpenGlFormat(UInt32 glFormat);
  }
}
