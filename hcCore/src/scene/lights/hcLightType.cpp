#include "hc/scene/lights/hcLightType.h"

namespace hc::scene::lights
{
  namespace lightType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Type::Directional:
        return "Directional";
      case Type::Omni:
        return "Omni";
      case Type::Spot:
        return "Spot";
      default:
        return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Directional")
        return Type::Directional;
      else if (str == "Omni")
        return Type::Omni;
      else if (str == "Spot")
        return Type::Spot;
      else
        throw std::invalid_argument("Invalid LightType string: " + str);
    }
  }
}
