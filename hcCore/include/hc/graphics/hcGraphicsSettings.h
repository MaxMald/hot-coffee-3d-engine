#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  struct GraphicsSettings
  {
    /**
     * @brief The type of render pipeline to use.
     */
    renderPipelineType::Type renderPipelineType = renderPipelineType::DeferredHybrid;
  };
}
