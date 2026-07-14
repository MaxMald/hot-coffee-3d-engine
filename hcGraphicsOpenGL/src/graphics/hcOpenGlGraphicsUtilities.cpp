#include "hc/graphics/hcOpenGlGraphicsUtilities.h"
#include <GL/glew.h>

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    void AssertOpenGlHasNoError()
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

    void CheckAndLogPossibleError()
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

    UInt32 GetOpenGlDrawModeFromDrawType(drawType::Type drawType)
    {
      switch (drawType)
      {
      case drawType::Triangles:
        return GL_TRIANGLES;
      case drawType::Lines:
        return GL_LINES;
      case drawType::LineStrip:
        return GL_LINE_STRIP;
      case drawType::LineLoop:
        return GL_LINE_LOOP;
      case drawType::Points:
        return GL_POINTS;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported draw type: %d", static_cast<int>(drawType))
        );
      }
    }

    drawType::Type GetDrawTypeFromOpenGlMode(UInt32 glMode)
    {
      switch (glMode)
      {
      case GL_TRIANGLES:
        return drawType::Triangles;
      case GL_LINES:
        return drawType::Lines;
      case GL_LINE_STRIP:
        return drawType::LineStrip;
      case GL_LINE_LOOP:
        return drawType::LineLoop;
      case GL_POINTS:
        return drawType::Points;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported OpenGL mode: %u", glMode)
        );
      }
    }

    UInt32 GetOpenGlPolygonModeFromPolygonFillType(polygonFillType::Type fillType)
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

    polygonFillType::Type GetPolygonFillTypeFromOpenGlPolygonMode(UInt32 glPolygonMode)
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

    UInt32 GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(
      textureFormatType::Type colorFormat,
      colorSpaceType::Type spaceColor
    )
    {
      switch (colorFormat)
      {
      case textureFormatType::RGBA8:
        return spaceColor == colorSpaceType::SRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
      case textureFormatType::RGB8:
        return spaceColor == colorSpaceType::SRGB ? GL_SRGB8 : GL_RGB8;
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
          String::Format("Unsupported color format type: %d", static_cast<int>(colorFormat))
        );
      }
    }

    UInt32 GetOpenGlFormatFromTextureFormatType(textureFormatType::Type textureFormat)
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
          String::Format("Unsupported texture format type: %d", static_cast<int>(textureFormat))
        );
      }
    }

    UInt32 GetOpenGLDataTypeFromTextureFormatType(textureFormatType::Type textureFormat)
    {
      switch (textureFormat)
      {
      case textureFormatType::RGBA8:
      case textureFormatType::RGB8:
        return GL_UNSIGNED_BYTE;
      case textureFormatType::RGB16F:
      case textureFormatType::RGBA16F:
      case textureFormatType::Depth16:
        return GL_UNSIGNED_SHORT;
      case textureFormatType::Depth24:
        return GL_UNSIGNED_INT;
      case textureFormatType::Depth32F:
        return GL_FLOAT;
      default:
        throw RuntimeErrorException(
          String::Format("Unsupported texture format type: %d", static_cast<int>(textureFormat))
        );
      }
    }
  }
}
