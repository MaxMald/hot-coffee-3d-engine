#include "hc/scene/camera/hcProjectionType.h"

namespace hc
{
  namespace projectionType
  {
    Type FromString(const String& str)
    {
      if (str == "Perspective")
      {
        return Type::Perspective;
      }
      else if (str == "Orthographic")
      {
        return Type::Orthographic;
      }
      else
      {
        throw RuntimeErrorException("Invalid projection type string: " + str);
      }
    }

    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Perspective:
        return "Perspective";
      case Type::Orthographic:
        return "Orthographic";
      default:
        throw RuntimeErrorException("Invalid projection type enum value");
      }
    }
  }
}
