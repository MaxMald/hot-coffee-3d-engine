#include "hc/graphics/resource/material/hcMaterialRenderMode.h"

namespace hc
{
  namespace materialRenderMode
  {
    String toString(Type renderMode)
    {
      switch (renderMode)
      {
        case Type::Background:
          return "Background";
        case Type::Opaque:
          return "Opaque";
        case Type::Transparent:
          return "Transparent";
        default:
          return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Background")
        return Type::Background;
      else if (str == "Opaque")
        return Type::Opaque;
      else if (str == "Transparent")
        return Type::Transparent;
      else
        throw RuntimeErrorException(
          String::Format("Invalid material render mode string: '%s'", str.c_str())
        );
    }
  }
}
