#pragma once

#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

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
     * @brief Initializes the shader with the specified stage type and source code.
     *
     * @param stageType The type of shader stage (e.g., vertex, fragment).
     * @param source The source code of the shader.
     */
    virtual void initialize(shaderStageType::Type stageType, const String& source) = 0;

    /**
     * @brief Returns the stage type of the shader.
     *
     * @return The shader stage type (e.g., vertex, fragment).
     */
    virtual shaderStageType::Type getStageType() const = 0;

    /**
     * @brief Checks if the shader is valid and can be used in a shader program.
     *
     * @return True if the shader is valid, false otherwise.
     */
    virtual bool isValid() const = 0;
  };
}
