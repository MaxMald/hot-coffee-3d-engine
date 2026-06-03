#include "hc/graphics/resource/shader/hcOpenGlShader.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlShader::OpenGlShader() :
    m_id(Id::Create()),
    m_stageType(shaderStageType::Vertex),
    m_valid(false),
    m_shaderId(0)
  {}

  OpenGlShader::~OpenGlShader()
  {
    destroy();
  }

  const Id& OpenGlShader::getId() const
  {
    return m_id;
  }

  void OpenGlShader::destroy()
  {
    if (m_shaderId != 0)
    {
      glDeleteShader(m_shaderId);
      m_shaderId = 0;
    }

    m_valid = false;
  }

  void OpenGlShader::initialize(shaderStageType::Type stageType, const String& source)
  {
    if (m_valid)
      throw RuntimeErrorException("Shader is already initialized.");

    try
    {
      GLenum glStage;
      switch (stageType)
      {
      case shaderStageType::Vertex:
        glStage = GL_VERTEX_SHADER;
        break;
      case shaderStageType::Fragment:
        glStage = GL_FRAGMENT_SHADER;
        break;
      default:
        throw RuntimeErrorException("Unsupported shader stage type.");
        return;
      }

      m_shaderId = static_cast<UInt32>(glCreateShader(glStage));
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      const char* src = source.c_str();
      glShaderSource(m_shaderId, 1, &src, nullptr);
      glCompileShader(m_shaderId);

      GLint compileStatus = GL_FALSE;
      glGetShaderiv(static_cast<GLuint>(m_shaderId), GL_COMPILE_STATUS, &compileStatus);
      if (compileStatus != GL_TRUE)
      {
        char log[1024];
        glGetShaderInfoLog(static_cast<GLuint>(m_shaderId), sizeof(log), nullptr, log);

        throw RuntimeErrorException("OpenGL shader compilation failed: " + String(log));
      }
    }
    catch (...)
    {
      destroy();
      throw;
    }

    m_stageType = stageType;
    m_valid = true;
  }

  shaderStageType::Type OpenGlShader::getStageType() const
  {
    return m_stageType;
  }

  bool OpenGlShader::isValid() const
  {
    return m_valid;
  }

  UInt32 OpenGlShader::getShaderId() const
  {
    return m_shaderId;
  }
}
