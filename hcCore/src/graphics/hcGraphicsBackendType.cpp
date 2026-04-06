#include "hc/graphics/hcGraphicsBackendType.h"

namespace hc
{
  namespace graphicsBackendType
  {
    String ToString(Type type)
    {
      switch (type)
      {
        case OPENGL:
          return "OpenGL";
        default:
          return "Unknown";
      }
    }

    Type FromString(const String& str)
    {
      if (str == "OpenGL")
        return OPENGL;
      else
        throw InvalidArgumentException("Invalid graphics backend type string: " + str);
    }
  }
}
