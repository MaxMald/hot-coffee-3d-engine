#include "hc/assets/hcPrimitiveShapeType.h"

namespace hc
{
  namespace primitiveShapeType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Cube:
        return "Cube";
      case Cone:
        return "Cone";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented primitive shape type:  %d", static_cast<Int32>(type))
        );
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Cube")
        return Cube;
      else if (str == "Cone")
        return Cone;
      else
        throw RuntimeErrorException("Cannot find primitive shape type for: " + str);
    }
  }
}
