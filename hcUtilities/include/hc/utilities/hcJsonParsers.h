#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcJson.h"

namespace hc
{
  struct Color;

  namespace jsonParsers
  {
    Color HC_UTILITY_EXPORT parseColor(const Json& json);
  }
}
