#pragma once

#include "hc/hcIGraphicResource.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  /**
   * @brief Interface for shader objects.
   *
   * Represents a shader with a specific stage type (e.g., vertex, fragment).
   * Provides methods for compilation, destruction, and querying shader state.
   */
  class IShader : public IGraphicResource
  {
  public:
    virtual ~IShader() = default;

    /**
     * @brief Returns the stage type of the shader.
     *
     * @return The shader stage type (e.g., vertex, fragment).
     */
    virtual shaderStageType::Type getStageType() const = 0;

    /**
     * @brief Checks if the shader is compiled.
     * 
     * @return True if the shader is compiled, false otherwise.
     */
    virtual bool isCompiled() const = 0;

    /**
     * @brief Compiles the shader.
     */
    virtual void compile() = 0;

    /**
     * @brief Destroys the shader and releases associated resources.
     */
    virtual void destroy() = 0;
  };
}
