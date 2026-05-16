#include "hc/graphics/hcOpenGlGraphicsUtilitites.h"
#include <GL/glew.h>

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    void checkAndLogPosibleError()
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

    UInt32 getOpenGlDrawModeFromDrawType(drawType::Type drawType)
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

    drawType::Type getDrawTypeFromOpenGlMode(UInt32 glMode)
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

    UInt32 getOpenGlPolygonModeFromPolygonFillType(polygonFillType::Type fillType)
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

    polygonFillType::Type getPolygonFillTypeFromOpenGlPolygonMode(UInt32 glPolygonMode)
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
  }
}
