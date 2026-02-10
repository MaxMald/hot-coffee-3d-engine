#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief Represents an OpenGL shader object.
   */
  class OpenGlShader : public IShader
  {
  public:
    /**
     * @brief Constructs an OpenGlShader with the given stage type and source
     * code.
     *
     * @param stageType The shader stage type (e.g., vertex, fragment).
     * @param source The GLSL source code for the shader.
     */
    OpenGlShader(shaderStageType::Type stageType, const String& source);
    ~OpenGlShader() override;

    /**
     * @brief Returns the unique identifier of the shader resource.
      *
      * @return Reference to the shader's Id.
     */
    const Id& getId() const override;

    /**
     * @brief Destroys the shader and releases OpenGL resources.
     */
    void destroy() override;

    /**
     * @brief Returns the stage type of the shader (vertex, fragment, etc.).
     */
    shaderStageType::Type getStageType() const override;

    /**
     * @brief Checks if the shader has been successfully compiled.
     */
    bool isCompiled() const override;

    /**
     * @brief Compiles the shader from its source code.
     */
    void compile() override;

    /**
     * @brief Returns the OpenGL shader object ID.
     *
     * @return The OpenGL shader ID.
     */
    GLuint getShaderId() const;

  private:
    Id m_id;
    shaderStageType::Type m_stageType;
    String m_source;
    bool m_compiled;
    GLuint m_shaderId;
  };
}
