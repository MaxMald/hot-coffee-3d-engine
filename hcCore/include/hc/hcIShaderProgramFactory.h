#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  class IShaderProgram;
  class IShader;

  /**
   * @brief Interface for shader program factory classes.
   *
   * Provides an abstract interface for creating shader program instances from a
   * collection of shaders. Implementations should provide concrete shader
   * program creation logic.
   */
  class HC_CORE_EXPORT IShaderProgramFactory
  {
  public:
    virtual ~IShaderProgramFactory() = default;

    /**
     * @brief Creates a shader program from the given collection of shaders.
     * 
     * @param shaders Vector of shared pointers to shader instances used in the
     * program.
     * 
     * @return Shared pointer to the created shader program instance. nullptr if
     * creation fails.
     */
    virtual SharedPtr<IShaderProgram> createShaderProgram(
      const Vector<SharedPtr<IShader>>& shaders
    ) = 0;

  protected:
    IShaderProgramFactory() = default;
  };
}
