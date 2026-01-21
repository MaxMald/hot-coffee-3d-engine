#pragma once

#include "hc/hcIJsonProperty.h"
#include "hc/hcString.h"

namespace hc
{
  /**
   * @brief Represents a formatted JSON value property.
   */
  class JsonFormattedValue : public IJsonProperty
  {
  public:
    /**
     * @brief Constructs a JsonFormattedValue with the specified value.
     *
     * @param value The value to be stored and formatted as JSON.
     */
    JsonFormattedValue(const String& value);
    virtual ~JsonFormattedValue();

    /**
     * @copydoc IJsonProperty::toString
     */
    virtual String toString() const override;

  private:
    String m_value;
  };
}
