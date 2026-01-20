#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShaderType.h"

namespace hc
{
  /**
   * @brief Interface for material objects in the engine.
   */
  class IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    /**
     * @brief Gets the shader type associated with this material.
     * 
     * @return The shader type.
     */
    virtual shaderType::Type getShaderType() const = 0;
  };
}
