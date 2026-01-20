#include "hc/hcShaderType.h"

namespace hc
{
  namespace shaderType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Unlit:
        return "Unlit";
      case BlinnPhong:
        return "BlinnPhong";
      default:
        return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Unlit")
        return Unlit;
      else if (str == "BlinnPhong")
        return BlinnPhong;
      else
        return Unknown;
    }
  }
}
