#include "hc/graphics/hcRenderPipelineType.h"

namespace hc
{
  namespace renderPipelineType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Forward:
        return "Forward";
      case Type::DeferredHybrid:
        return "DeferredHybrid";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented render pipeline type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Forward")
        return Type::Forward;
      else if (str == "DeferredHybrid")
        return Type::DeferredHybrid;
      else
        throw InvalidArgumentException(
          String::Format("Invalid render pipeline type string: %s", str.c_str())
        );
    }
  }
}
