#pragma once

#include "hc/scene/light/hcALight.h"

namespace hc
{
  namespace dataBlockStructure
  {
    struct OmniLight;
  }

  class HC_CORE_EXPORT OmniLight : public ALight
  {
  public:
    OmniLight();

    ~OmniLight() override = default;

    /**
     * @brief Gets the GPU-aligned data structure representing this omni light.
     *
     * @return A dataBlockStructure::OmniLight containing the light's data.
     */
    dataBlockStructure::OmniLight getDataBlockStructure() const;
  };
}
