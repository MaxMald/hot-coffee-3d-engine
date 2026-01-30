#pragma once

#include <hc/hcIShaderProgram.h>
#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
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
    void setUniform(const String& name, const float* matrix4x4) override;

    /**
     * @copydoc IShaderProgram::destroy
     */
    void destroy() override;

    bool link();

  private:
    GLuint m_programId;
    bool m_linked;
    UnorderedMap<String, GLint> m_uniformLocationCache;

    GLint getUniformLocation(const String& name);
  };
}
