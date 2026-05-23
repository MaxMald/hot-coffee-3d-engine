#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IShaderProgram;

  /**
   * @brief Interface for shader program manager classes.
   *
   * Provides methods for accessing and managing shader program instances.
   * Implementations should supply logic for retrieving default programs and
   * clearing resources.
   */
  class HC_CORE_EXPORT IShaderProgramManager : public NonCopyable
  {
  public:
    virtual ~IShaderProgramManager() = default;

    /**
     * @brief Retrieves the default unlit shader program.
     *
     * @return Shared pointer to the unlit shader program instance.
     */
    virtual SharedPtr<IShaderProgram> getUnlitShaderProgram() = 0;

    /**
     * @brief Retrieves the default Blinn-Phong shader program for forward rendering.
     *
     * @return Shared pointer to the Blinn-Phong forward shader program instance.
     */
    virtual SharedPtr<IShaderProgram> getBlinnPhongForwardProgram() = 0;

    /**
     * @brief Clears all managed shader programs and releases associated
     * resources.
     */
    virtual void clear() = 0;
  };
}
