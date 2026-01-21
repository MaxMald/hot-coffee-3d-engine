#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"

namespace hc
{
  /**
   * @brief Interface for JSON property objects.
   *
   * IJsonProperty defines the contract for all JSON property types, providing a
   * method to convert the property to its JSON string representation.
   * Implementations may represent primitive values, objects, arrays, or other
   * JSON-compatible structures.
   */
  class IJsonProperty
  {
  public:
    virtual ~IJsonProperty() = default;

    /**
     * @brief Converts the property to its JSON string representation.
     *
     * @return The JSON-formatted string.
     */
    virtual String toString() const = 0;
  };
}
