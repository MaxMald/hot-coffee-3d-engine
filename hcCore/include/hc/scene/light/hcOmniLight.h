#pragma once

#include "hc/scene/light/hcALight.h"

namespace hc
{
  class HC_CORE_EXPORT OmniLight : public ALight
  {
  public:
    OmniLight();

    ~OmniLight() override = default;
  };
}
