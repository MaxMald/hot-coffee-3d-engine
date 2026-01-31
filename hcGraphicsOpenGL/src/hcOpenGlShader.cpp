#include "hc/hcOpenGlShader.h"

namespace hc
{
  OpenGlShader::OpenGlShader(shaderStageType::Type stageType, const String& source)
    : m_stageType(stageType),
    m_source(source),
    m_compiled(false),
    m_shaderId(0)
  {
  }

  OpenGlShader::~OpenGlShader()
  {
    destroy();
  }

  shaderStageType::Type OpenGlShader::getStageType() const
  {
    return m_stageType;
  }

  bool OpenGlShader::isCompiled() const
  {
    return m_compiled;
  }

  void OpenGlShader::compile()
  {
    if (m_compiled)
      return;

    GLenum glStage;
    switch (m_stageType)
    {
    case shaderStageType::Vertex:
      glStage = GL_VERTEX_SHADER;
      break;
    case shaderStageType::Fragment:
      glStage = GL_FRAGMENT_SHADER;
      break;
    default:
      LogService::Error(
        "Unsupported shader stage type for OpenGL shader compilation."
      );
      return;
    }

    m_shaderId = glCreateShader(glStage);
    const char* src = m_source.c_str();
    glShaderSource(m_shaderId, 1, &src, nullptr);
    glCompileShader(m_shaderId);

    GLint compileStatus = GL_FALSE;
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
      char log[1024];
      glGetShaderInfoLog(m_shaderId, sizeof(log), nullptr, log);

      LogService::Error(
        "OpenGL shader compilation failed: " + String(log)
      );

      glDeleteShader(m_shaderId);
      m_shaderId = 0;
      m_compiled = false;
      return;
    }

    m_compiled = true;
  }

  void OpenGlShader::destroy()
  {
    if (m_shaderId != 0)
    {
      glDeleteShader(m_shaderId);
      m_shaderId = 0;
    }

    m_compiled = false;
  }

  GLuint OpenGlShader::getShaderId() const
  {
    return m_shaderId;
  }
}
