#include "hc/graphics/hcPolygonFillType.h"

namespace hc
{
  namespace polygonFillType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Undefined:
        return "Undefined";
      case Type::Solid:
        return "Solid";
      case Type::Wireframe:
        return "Wireframe";
      case Type::Point:
        return "Point";
      default:
        return "Unknown";
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Undefined")
        return Type::Undefined;
      else if (str == "Solid")
        return Type::Solid;
      else if (str == "Wireframe")
        return Type::Wireframe;
      else if (str == "Point")
        return Type::Point;
      else
        throw InvalidArgumentException("Invalid polygon fill type string: " + str);
    }
  }
}
