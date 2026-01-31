#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlShader : public IShader
  {
  public:
    OpenGlShader(shaderStageType::Type stageType, const String& source);
    ~OpenGlShader() override;

    /**
     * @copydoc IShader::getStageType
     */
    shaderStageType::Type getStageType() const override;

    /**
     * @copydoc IShader::isCompiled
     */
    bool isCompiled() const override;

    /**
     * @copydoc IShader::compile
     */
    void compile() override;

    /**
     * @copydoc IShader::destroy
     */
    void destroy() override;

    GLuint getShaderId() const;

  private:
    shaderStageType::Type m_stageType;
    String m_source;
    bool m_compiled;
    GLuint m_shaderId;
  };
}
