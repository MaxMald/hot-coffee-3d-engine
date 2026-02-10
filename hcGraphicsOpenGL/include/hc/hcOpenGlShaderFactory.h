#pragma once

#include <hc/hcIShaderFactory.h>

namespace hc
{
  /**
   * @brief Factory for creating OpenGL shader instances.
   *
   * Implements the IShaderFactory interface to construct OpenGL shader objects
   * from source code strings or by providing default shader implementations.
   */
  class OpenGlShaderFactory : public IShaderFactory
  {
  public:
    OpenGlShaderFactory() = default;
    ~OpenGlShaderFactory() override = default;

    /**
     * @brief Creates an OpenGL shader from the given source string and shader
     * stage type.
     * 
     * @param content The GLSL source code for the shader.
     * @param type The shader stage type (e.g., vertex, fragment).
     * 
     * @return Shared pointer to the created IShader instance.
     */
    SharedPtr<IShader> createShaderFromStringContent(
      const String& content,
      shaderStageType::Type type
    ) override;

    /**
     * @brief Creates a default OpenGL vertex shader.
     * 
     * @return Shared pointer to the created IShader instance.
     */
    SharedPtr<IShader> createDefaultVertexShader() override;

    /**
     * @brief Creates a default OpenGL unlit fragment shader.
     * 
     * @return Shared pointer to the created IShader instance.
     */
    SharedPtr<IShader> createUnlitFragmentShader() override;
  };
}
