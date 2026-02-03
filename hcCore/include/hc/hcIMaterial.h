#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShadingType.h"

namespace hc
{
  /**
   * @brief Interface for material objects in the engine.
   */
  class HC_CORE_EXPORT IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    /**
     * @brief Gets the shader type associated with this material.
     * 
     * @return The shader type.
     */
    virtual shadingType::Type getShaderType() const = 0;

  protected:
    IMaterial() = default;
  };
}
