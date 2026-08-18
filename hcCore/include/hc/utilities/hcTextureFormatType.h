#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace textureFormatType
  {
    enum Type : UInt8
    {
      // Color formats
      RGB8,
      RGBA8,
      RGB16F,
      RGBA16F,

      // Depth formats
      Depth16,
      Depth24,
      Depth32F,

      Count,
      Undefined
    };

    inline UInt8 GetChannelCount(Type format)
    {
      switch (format)
      {
      case RGBA8:
      case RGBA16F:
        return 4;
      case RGB8:
      case RGB16F:
        return 3;
      case Depth16:
      case Depth24:
      case Depth32F:
        return 1;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getChannelCount for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    inline UInt8 GetBitsPerChannel(Type format)
    {
      switch (format)
      {
      case RGBA8:
      case RGB8:
        return 8;
      case RGBA16F:
      case RGB16F:
      case Depth16:
        return 16;
      case Depth24:
        return 24;
      case Depth32F:
        return 32;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getBitsPerChannel for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    inline String ToString(Type format)
    {
      switch (format)
      {
      case RGBA8:
        return "RGBA8";
      case RGB8:
        return "RGB8";
      case RGBA16F:
        return "RGBA16F";
      case RGB16F:
        return "RGB16F";
      case Depth16:
        return "Depth16";
      case Depth24:
        return "Depth24";
      case Depth32F:
        return "Depth32F";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: toString for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    inline Type FromString(const String& str)
    {
      if (str == "RGBA8")
        return RGBA8;
      if (str == "RGB8")
        return RGB8;
      if (str == "RGB16F")
        return RGB16F;
      if (str == "RGBA16F")
        return RGBA16F;
      if (str == "Depth16")
        return Depth16;
      if (str == "Depth24")
        return Depth24;
      if (str == "Depth32F")
        return Depth32F;
      throw RuntimeErrorException(
        String::Format("Not implemented: fromString for texture format type string '%s'", str.c_str()));
    }
  }
}
