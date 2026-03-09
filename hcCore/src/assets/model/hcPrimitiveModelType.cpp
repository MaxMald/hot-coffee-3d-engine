#include "hc/assets/model/hcPrimitiveModelType.h"

namespace hc
{
  namespace primitiveModelType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Undefined:
        return "Undefined";
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
      case Count:
        return "Count";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented primitive model type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Undefined")
        return Undefined;
      else if (str == "Cube")
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
      else if (str == "Count")
        return Count;
      else
        throw RuntimeErrorException("Cannot find primitive model type for: " + str);
    }
  }
}
