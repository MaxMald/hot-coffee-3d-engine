#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"

namespace hc
{
  class IShaderProgram;

  /**
   * @brief Interface for shader program factory classes.
   *
   * Provides an abstract interface for creating shader program instances from a
   * collection of shaders. Implementations should provide concrete shader
   * program creation logic.
   */
  class IShaderProgramFactory
  {
  public:
    virtual ~IShaderProgramFactory() = default;

    /**
     * @brief Creates a shader program from the given collection of shaders.
     * 
     * @return Shared pointer to the created shader program instance. nullptr if
     * creation fails.
     */
    virtual SharedPtr<IShaderProgram> createShaderProgram() = 0;

  protected:
    IShaderProgramFactory() = default;
  };
}
