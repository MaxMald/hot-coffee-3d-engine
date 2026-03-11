#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace bitUtilities
  {
    /**
     * Flips the bits of a floating-point number represented as an unsigned integer
     * to allow for correct sorting of floating-point values when treated as integers.
     *
     * @param floatBits The bits of the floating-point number as an unsigned integer.
     *
     * @return The flipped bits that can be used for sorting.
     */
    inline UInt32 floatFlip(UInt32 floatBits)
    {
      UInt32 mask = -static_cast<Int32>(floatBits >> 31) | 0x80000000;
      return floatBits ^ mask;
    }
  }
}
