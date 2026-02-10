#pragma once

#include <hc/hcIShaderProgramFactory.h>

namespace hc
{
  /**
   * @brief Factory for creating OpenGL shader program instances.
   *
   * Implements the IShaderProgramFactory interface to construct OpenGL shader
   * program objects.
   */
  class OpenGlShaderProgramFactory : public IShaderProgramFactory
  {
  public:
    OpenGlShaderProgramFactory() = default;
    ~OpenGlShaderProgramFactory() override = default;

    /**
     * @brief Creates a new OpenGL shader program instance.
     *
     * @return Shared pointer to the created IShaderProgram instance.
     */
    SharedPtr<IShaderProgram> createShaderProgram() override;
  };
}
