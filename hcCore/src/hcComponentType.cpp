#include "hc/hcComponentType.h"

namespace hc
{
  namespace componentType
  {
    Type FromString(const String& str)
    {
      if (str == "Mesh")
        return Mesh;
      else if (str == "Light")
        return Light;
      else if (str == "Camera")
        return Camera;
      else
        return Unknown;
    }

    String ToString(Type type)
    {
      switch (type)
      {
      case Mesh:
        return "Mesh";
      case Light:
        return "Light";
      case Camera:
        return "Camera";
      default:
        return "Unknown";
      }
    }
  }
}
