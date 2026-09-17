#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcJson.h"
#include "hc/utilities/hcPath.h"

namespace hc
{
  struct Color;

  namespace jsonParsers
  {
    Color HC_UTILITY_EXPORT parseColor(const Json& json);
    Path HC_UTILITY_EXPORT parsePath(const Json& json);
  }
}
