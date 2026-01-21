#include "hc/hcJsonFromattedValue.h"

namespace hc
{
  JsonFormattedValue::JsonFormattedValue(const String& value) :
    m_value(value)
  {
  }

  JsonFormattedValue::~JsonFormattedValue()
  {
  }

  String JsonFormattedValue::toString() const
  {
    return m_value;
  }
}
