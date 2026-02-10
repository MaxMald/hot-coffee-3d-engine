#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief OpenGL implementation of the IShaderProgram interface.
   */
  class OpenGlShaderProgram : public IShaderProgram
  {
  public:
    OpenGlShaderProgram();
    ~OpenGlShaderProgram() override;

    /**
     * @copydoc IShaderProgram::bind
     */
    void bind() override;

    /**
     * @copydoc IShaderProgram::linkShaders
     */
    void linkShaders() override;

    /**
     * @copydoc IShaderProgram::isValid
     */
    bool isValid() const override;

    /**
     * @copydoc IShaderProgram::attachShader
     */
    void attachShader(SharedPtr<IShader> shader) override;

    /**
     * @copydoc IShaderProgram::setUniform
     */
    void setUniform(const String& name, float value) override;

    /**
     * @copydoc IShaderProgram::setUniform
     */
    void setUniform(const String& name, Int32 value) override;

    /**
    * @copydoc IShaderProgram::setUniform
    */
    void setUniform(const String& name, bool value) override;

    /*
    * @copydoc IShaderProgram::setUniform
    */
    void setUniform(const String& name, const Vector3f& v3f) override;

    /**
     * @copydoc IShaderProgram::setUniform
     */
    void setUniform(const String& name, const Color& color) override;

    /**
     * @copydoc IShaderProgram::setUniform
     */
    void setUniform(const String& name, const Matrix4& matrix4x4) override;

    /**
     * @copydoc IShaderProgram::setUniform
     */
    void setUniformTexture(const String& name, Int32 slotLocation) override;

    /**
     * @copydoc IShaderProgram::destroy
     */
    void destroy() override;

  private:
    GLuint m_programId;
    bool m_linked;
    UnorderedMap<String, GLint> m_uniformLocationCache;

    GLint getUniformLocation(const String& name);
  };
}
