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
      case Sphere:
        return "Sphere";
      case Cylinder:
        return "Cylinder";
      case Plane:
        return "Plane";
      case Pyramid:
        return "Pyramid";
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
      else if (str == "Sphere")
        return Sphere;
      else if (str == "Cylinder")
        return Cylinder;
      else if (str == "Plane")
        return Plane;
      else if (str == "Pyramid")
        return Pyramid;
      else
        throw RuntimeErrorException("Cannot find primitive shape type for: " + str);
    }
  }
}
