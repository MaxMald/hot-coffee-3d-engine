#include "hc/graphics/resource/shader/hcOpenGlShader.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  static GLenum ToOpenGlShaderStageType(shaderStageType::Type type)
  {
    switch (type)
    {
    case shaderStageType::Vertex:
      return GL_VERTEX_SHADER;
    case shaderStageType::Fragment:
      return GL_FRAGMENT_SHADER;
    default:
      throw RuntimeErrorException(
        String::Format("Unsupported shader stage type: %d", static_cast<int>(type))
      );
    }
  }

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
      GLenum glStage = ToOpenGlShaderStageType(stageType);

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

  void OpenGlShader::initializeFromSpirv(
    shaderStageType::Type stageType,
    const Vector<UInt32>& spirvCode,
    const String& entryPoint
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Shader is already initialized.");

    try
    {
      GLenum glStage = ToOpenGlShaderStageType(stageType);

      m_shaderId = static_cast<UInt32>(glCreateShader(glStage));
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glShaderBinary(
        1,
        &m_shaderId,
        GL_SHADER_BINARY_FORMAT_SPIR_V,
        spirvCode.data(),
        spirvCode.size() * sizeof(UInt32)
      );

      glSpecializeShader(
        m_shaderId,
        entryPoint.c_str(),
        0,
        nullptr,
        nullptr
      );

      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      destroy();
      throw;
    }

    m_stageType = stageType;
    m_valid = true;
  }

  UInt32 OpenGlShader::getShaderId() const
  {
    return m_shaderId;
  }
}
