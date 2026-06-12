#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace colorSpaceType
  {
    enum Type : UInt8
    {
      SRGB,
      Linear,
      Count
    };

    inline String ToString(Type colorSpace)
    {
      switch (colorSpace)
      {
      case SRGB:
        return "SRGB";
      case Linear:
        return "Linear";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: toString for color space type %d", static_cast<UInt8>(colorSpace)));
      }
    }

    inline Type FromString(const String& colorSpaceStr)
    {
      if (colorSpaceStr == "SRGB")
        return SRGB;
      else if (colorSpaceStr == "Linear")
        return Linear;
      else
        throw RuntimeErrorException(
          String::Format("Not implemented: getColorSpaceTypeFromString for color space type string '%s'", colorSpaceStr.c_str()));
    }
  }
}
