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
        return OmniLight;
      else if (str == "SpotLight")
        return SpotLight;
      else if (str == "DirectionalLight")
        return DirectionalLight;
      else
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
      case OmniLight:
        return "OmniLight";
      case SpotLight:
        return "SpotLight";
      case DirectionalLight:
        return "DirectionalLight";
      default:
        return "Unknown";
      }
    }
  }
}
