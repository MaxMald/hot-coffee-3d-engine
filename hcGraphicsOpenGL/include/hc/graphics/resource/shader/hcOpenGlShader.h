#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief Represents an OpenGL shader object.
   */
  class OpenGlShader : public IShader
  {
  public:
    OpenGlShader();
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
     * @brief Initializes the shader with the specified stage type and source code.
     *
     * Compiles the shader source code and creates an OpenGL shader object.
     *
     * @param stageType The type of shader stage (e.g., vertex, fragment).
     * @param source The source code of the shader.
     */
    void initialize(shaderStageType::Type stageType, const String& source) override;

    /**
     * @copydoc IShader::initialize
     */
    shaderStageType::Type getStageType() const override;

    /**
     * @copydoc IShader::isValid
     */
    bool isValid() const override;


    /**
     * @brief Initializes the shader with the specified stage type and SPIR-V code.
     *
     * @param stageType The type of shader stage (e.g., vertex, fragment).
     * @param spirvCode The SPIR-V bytecode of the shader.
     * @param entryPoint The entry point function name in the SPIR-V code.
     */
    void initializeFromSpirv(
      shaderStageType::Type stageType,
      const Vector<UInt32>& spirvCode,
      const String& entryPoint
    );

    /**
     * @brief Returns the OpenGL shader object ID.
     *
     * @return The OpenGL shader ID.
     */
    UInt32 getShaderId() const;

  private:
    Id m_id;
    shaderStageType::Type m_stageType;
    bool m_valid;
    UInt32 m_shaderId;
  };
}
