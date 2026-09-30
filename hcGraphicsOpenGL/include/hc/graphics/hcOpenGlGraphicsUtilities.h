#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    /**
     * @brief Asserts that there are no OpenGL errors.
     */
    inline void AssertOpenGlHasNoError()
    {
      GLenum error = glGetError();
      if (error != GL_NO_ERROR)
      {
        String errorMessage;
        switch (error)
        {
        case GL_INVALID_ENUM:                  errorMessage = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errorMessage = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errorMessage = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errorMessage = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errorMessage = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errorMessage = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errorMessage = "INVALID_FRAMEBUFFER_OPERATION"; break;
        default: errorMessage = "UNKNOWN_ERROR"; break;
        }

        throw RuntimeErrorException(
          String::Format("OpenGL error: %s", errorMessage.c_str())
        );
      }
    }

    /**
    * @brief Checks for OpenGL errors and logs them if any are found.
    */
    inline void CheckAndLogPossibleError()
    {
      GLenum error = glGetError();
      if (error != GL_NO_ERROR)
      {
        LogService::Error(
          String("OpenGL error: ") +
          String(reinterpret_cast<const char*>(glewGetErrorString(error)))
        );
      }
    }

    /**
     * @brief Converts a topology type to the corresponding OpenGL draw mode.
     * @param topologyType The topology type to convert.
     * @return The corresponding OpenGL draw mode.
     */
    inline GLenum GetOpenGlDrawModeFromTopologyType(topologyType::Type topologyType)
    {
      switch (topologyType)
      {
      case topologyType::Triangles:
        return GL_TRIANGLES;
      case topologyType::Lines:
        return GL_LINES;
      case topologyType::LineStrip:
        return GL_LINE_STRIP;
      case topologyType::LineLoop:
        return GL_LINE_LOOP;
      case topologyType::Points:
        return GL_POINTS;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported topology type: %d", static_cast<int>(topologyType))
        );
      }
    }

    /**
     * @brief Converts an OpenGL draw mode to the corresponding topology type.
     * @param glMode The OpenGL draw mode to convert.
     * @return The corresponding topology type.
     */
    inline topologyType::Type GetTopologyTypeFromOpenGlMode(GLenum glMode)
    {
      switch (glMode)
      {
      case GL_TRIANGLES:
        return topologyType::Triangles;
      case GL_LINES:
        return topologyType::Lines;
      case GL_LINE_STRIP:
        return topologyType::LineStrip;
      case GL_LINE_LOOP:
        return topologyType::LineLoop;
      case GL_POINTS:
        return topologyType::Points;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported OpenGL mode: %u", glMode)
        );
      }
    }

    /**
     * @brief Converts a polygon fill type to the corresponding OpenGL polygon mode.
     * @param fillType The polygon fill type to convert.
     * @return The corresponding OpenGL polygon mode.
     */
    inline GLenum GetOpenGlPolygonModeFromPolygonFillType(polygonFillType::Type fillType)
    {
      switch (fillType)
      {
      case polygonFillType::Solid:
        return GL_FILL;
      case polygonFillType::Wireframe:
        return GL_LINE;
      case polygonFillType::Point:
        return GL_POINT;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported polygon fill type: %d", static_cast<int>(fillType))
        );
      }
    }
    
    /**
     * @brief Converts an OpenGL polygon mode to the corresponding polygon fill type.
     * @param glPolygonMode The OpenGL polygon mode to convert.
     * @return The corresponding polygon fill type.
     */
    inline polygonFillType::Type GetPolygonFillTypeFromOpenGlPolygonMode(
      GLenum glPolygonMode
    )
    {
      switch (glPolygonMode)
      {
      case GL_FILL:
        return polygonFillType::Solid;
      case GL_LINE:
        return polygonFillType::Wireframe;
      case GL_POINT:
        return polygonFillType::Point;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported OpenGL polygon mode: %u", glPolygonMode)
        );
      }
    }

    /**
     * @brief Gets the OpenGL internal format from a texture format type and color space
     * type.
     * @param textureFormat The texture format type.
     * @param colorSpace The color space type.
     * @return The corresponding OpenGL internal format.
     */
    inline GLint GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(
      textureFormatType::Type textureFormat,
      colorSpaceType::Type colorSpace
    )
    {
      switch (textureFormat)
      {
      case textureFormatType::RGBA8:
        return colorSpace == colorSpaceType::SRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
      case textureFormatType::RGB8:
        return colorSpace == colorSpaceType::SRGB ? GL_SRGB8 : GL_RGB8;
      case textureFormatType::RGB16F:
        return GL_RGB16F;
      case textureFormatType::RGBA16F:
        return GL_RGBA16F;
      case textureFormatType::Depth16:
        return GL_DEPTH_COMPONENT16;
      case textureFormatType::Depth24:
        return GL_DEPTH_COMPONENT24;
      case textureFormatType::Depth32F:
        return GL_DEPTH_COMPONENT32F;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported color format type: %d", static_cast<int>(textureFormat))
        );
      }
    }

    /**
     * @brief Converts a texture format type to the corresponding OpenGL format.
     * @param textureFormat The texture format type to convert.
     * @return The corresponding OpenGL format.
     */
    inline GLenum GetOpenGlFormatFromTextureFormatType(
      textureFormatType::Type textureFormat
    )
    {
      switch (textureFormat)
      {
      case textureFormatType::RGBA8:
        return GL_RGBA;
      case textureFormatType::RGB8:
        return GL_RGB;
      case textureFormatType::RGB16F:
        return GL_RGB;
      case textureFormatType::RGBA16F:
        return GL_RGBA;
      case textureFormatType::Depth16:
      case textureFormatType::Depth24:
      case textureFormatType::Depth32F:
        return GL_DEPTH_COMPONENT;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported texture format type: %d", static_cast<Int32>(textureFormat))
        );
      }
    }
    
    /**
     * @brief Gets the OpenGL data type from a texture format type.
     * @param textureFormat The texture format type.
     * @return The corresponding OpenGL data type.
     */
    inline GLenum GetOpenGLDataTypeFromTextureFormatType(
      textureFormatType::Type textureFormat
    )
    {
      switch (textureFormat)
      {
      case textureFormatType::RGBA8:
      case textureFormatType::RGB8:
        return GL_UNSIGNED_BYTE;
      case textureFormatType::RGB16F:
      case textureFormatType::RGBA16F:
        return GL_HALF_FLOAT;
      case textureFormatType::Depth16:
        return GL_UNSIGNED_SHORT;
      case textureFormatType::Depth24:
        return GL_UNSIGNED_INT;
      case textureFormatType::Depth32F:
        return GL_FLOAT;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported texture format type: %d", static_cast<Int32>(textureFormat))
        );
      }
    }
  }
}
