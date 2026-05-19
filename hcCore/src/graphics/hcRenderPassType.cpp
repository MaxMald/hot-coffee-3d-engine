#include "hc/graphics/hcRenderPassType.h"

namespace hc
{
  namespace renderPassType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Forward:
        return "Forward";
      case Type::DeferredGeometry:
        return "DeferredGeometry";
      case Type::DeferredLighting:
        return "DeferredLighting";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented render pass type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Forward")
        return Type::Forward;
      else if (str == "DeferredGeometry")
        return Type::DeferredGeometry;
      else if (str == "DeferredLighting")
        return Type::DeferredLighting;
      else
        throw InvalidArgumentException(
          String::Format("Invalid render pass type string: %s", str.c_str())
        );
  }
}
