#include "hc/graphics/resource/material/hcShadingType.h"

namespace hc
{
  namespace shadingType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Unlit:
        return "Unlit";
      case BlinnPhong:
        return "BlinnPhong";
      case Hair:
        return "Hair";
      default:
        throw RuntimeErrorException("Unknown shading type.");
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Unlit")
        return Unlit;
      else if (str == "BlinnPhong")
        return BlinnPhong;
      else if (str == "Hair")
        return Hair;
      else
        throw RuntimeErrorException("Unknown shading type string: " + str);
    }
  }
}
