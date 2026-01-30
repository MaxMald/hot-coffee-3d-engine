#include "hc/hcShaderStageType.h"

namespace hc
{
  namespace shaderStageType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Vertex:
        return "Vertex";
      case Fragment:
        return "Fragment";
      default:
        return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Vertex")
        return Vertex;
      else if (str == "Fragment")
        return Fragment;
      else
        return Unknown;
    }
  }
}
