#pragma once

#include "hc/scene/light/hcALight.h"
#include "hc/graphics/lightFrameData/hcOmniLightFrameData.h"

namespace hc
{
  class HC_CORE_EXPORT OmniLight : public ALight
  {
  public:
    OmniLight();

    ~OmniLight() override = default;

    OmniLightFrameData toFrameData() const;
  };
}
