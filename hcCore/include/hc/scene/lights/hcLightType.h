#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace lightType
  {
    enum Type : UInt8
    {
      Directional,
      Omni,
      Spot
    };

    /**
     * @brief Converts a LightType to its string representation.
     */
    String HC_CORE_EXPORT toString(Type type);

    /**
     * @brief Parses a string to obtain the corresponding LightType.
     *
     * @param str The string representation of the LightType (e.g., "Directional",
     * "Omni", "Spot").
     * @return The corresponding LightType enum value.
     *
     * @throws std::invalid_argument if the input string does not match any valid
     * LightType.
     */
    Type HC_CORE_EXPORT fromString(const String& str);
  }
}
