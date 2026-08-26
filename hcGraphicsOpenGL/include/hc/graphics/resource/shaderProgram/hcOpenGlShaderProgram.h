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
     * @copydoc IGraphicResource::getId
     */
    const Id& getId() const override;

    /**
     * @copydoc IShaderProgram::initialize
     */
    void initialize(const Vector<SharedPtr<IShader>>& shaders) override;

    /**
     * @copydoc IShaderProgram::bind
     */
    void bind() override;

    /**
     * @copydoc IShaderProgram::isValid
     */
    bool isValid() const override;

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
    void setUniform(const String& name, const Vector4f& v4f) override;

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
    Id m_id;
    GLuint m_programId;
    bool m_valid;
    UnorderedMap<String, GLint> m_uniformLocationCache;

    void assertIsValid() const;
    GLint getUniformLocation(const String& name);
    void attachShader(SharedPtr<IShader> shader);
  };
}
