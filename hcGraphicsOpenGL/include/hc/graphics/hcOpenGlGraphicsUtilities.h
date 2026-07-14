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
     * @brief Gets the OpenGL internal format from a texture format type and color space
     * type.
     */
    UInt32 GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(
      textureFormatType::Type textureFormat,
      colorSpaceType::Type colorSpace
    );

    /**
     * @brief Converts a texture format type to the corresponding OpenGL format.
     */
    UInt32 GetOpenGlFormatFromTextureFormatType(textureFormatType::Type textureFormat);
    
    /**
     * @brief Gets the OpenGL data type from a texture format type.
     */
    UInt32 GetOpenGLDataTypeFromTextureFormatType(textureFormatType::Type textureFormat);
  }
}
