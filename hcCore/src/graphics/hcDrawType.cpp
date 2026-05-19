#include "hc/graphics/hcDrawType.h"

namespace hc
{
  namespace drawType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Triangles:
        return "Triangles";
      case Type::Lines:
        return "Lines";
      case Type::LineStrip:
        return "LineStrip";
      case Type::LineLoop:
        return "LineLoop";
      case Type::Points:
        return "Points";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented draw type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Triangles")
        return Type::Triangles;
      else if (str == "Lines")
        return Type::Lines;
      else if (str == "LineStrip")
        return Type::LineStrip;
      else if (str == "LineLoop")
        return Type::LineLoop;
      else if (str == "Points")
        return Type::Points;
      else
        throw InvalidArgumentException("Invalid draw type: " + str);
    }
  }
}
