#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcJson.h"

namespace hc
{
  struct Color;

  namespace jsonParsers
  {
    Color HC_UTILITY_EXPORT parseColor(const Json& json);
  }
}
