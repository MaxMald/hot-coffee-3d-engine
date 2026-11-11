#include "hc/scene/gameObject/components/hcComponentType.h"

namespace hc
{
  namespace componentType
  {
    Type FromString(const String& str)
    {
      if (str == "Mesh")
        return Mesh;
      else if (str == "Camera")
        return Camera;
      else if (str == "OmniLight")
        return Light;
      else if (str == "Light")
        return Unknown;
    }

    String ToString(Type type)
    {
      switch (type)
      {
      case Mesh:
        return "Mesh";
      case Camera:
        return "Camera";
      case Light:
        return "Light";
      default:
        return "Unknown";
      }
    }
  }
}
