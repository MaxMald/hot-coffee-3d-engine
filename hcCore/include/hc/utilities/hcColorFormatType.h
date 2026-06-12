#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace colorFormatType
  {
    enum Type : UInt8
    {
      RGBA8,
      RGB8,
      RGB16F,
      RGBA16F,
      Count
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
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getChannelCount for color format type %d", static_cast<UInt8>(format)));
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
        return 16;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getBitsPerChannel for color format type %d", static_cast<UInt8>(format)));
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
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: toString for color format type %d", static_cast<UInt8>(format)));
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
      throw RuntimeErrorException(
        String::Format("Not implemented: fromString for color format type string '%s'", str.c_str()));
    }
  }
}
