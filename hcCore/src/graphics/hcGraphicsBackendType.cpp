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
          throw RuntimeErrorException(
            String::Format("Not implemented graphics backend type: %d", static_cast<Int32>(type))
          );
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
