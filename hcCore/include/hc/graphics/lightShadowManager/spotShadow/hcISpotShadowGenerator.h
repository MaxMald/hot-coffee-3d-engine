#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/lights/hcLight.h"

namespace hc::graphics::shadows::spotShadow
{
  class HC_CORE_EXPORT ISpotShadowGenerator
  {
  public:
    virtual ~ISpotShadowGenerator();

    

  protected:
    ISpotShadowGenerator();
  };
}
