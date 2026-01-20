#include "hc/hcLightType.h"

namespace hc
{
  namespace lightType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Type::Directional:
        return "Directional";
      case Type::Point:
        return "Point";
      case Type::Spot:
        return "Spot";
      default:
        return "Unknown";
      }
    }
  }
}
