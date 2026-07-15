#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/ubos/hcUniformBufferObject.h"

namespace hc
{
  class LightShadowManager
  {
  public:
    LightShadowManager();
    virtual ~LightShadowManager();

    void initialize(UInt32 bindingPoint);
    void destroy();

  private:
    LightShadowFrameUBO m_lightShadowUBO;
  };
}
