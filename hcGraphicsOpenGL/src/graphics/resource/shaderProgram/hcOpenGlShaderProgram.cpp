#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgram.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlShaderProgram::OpenGlShaderProgram() :
    m_id(Id::Create()),
    m_programId(0),
    m_valid(false)
  {
  }

  OpenGlShaderProgram::~OpenGlShaderProgram()
  {
    destroy();
  }

  const Id& OpenGlShaderProgram::getId() const
  {
    return m_id;
  }

  void OpenGlShaderProgram::initialize(const Vector<SharedPtr<IShader>>& shaders)
  {
    if (m_valid)
      throw RuntimeErrorException(
        "Attempted to initialize an already initialized shader program. Each shader program instance can only be initialized once."
      );

    try
    {
      m_programId = glCreateProgram();
      if (m_programId == 0)
        throw RuntimeErrorException("Failed to create OpenGL shader program.");

      for (const auto& shader : shaders)
        attachShader(shader);

      glLinkProgram(m_programId);
      GLint linkStatus = GL_FALSE;
      glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);

      if (linkStatus == GL_FALSE)
      {
        GLint infoLogLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
          Vector<char> infoLog(infoLogLength);
          glGetProgramInfoLog(m_programId, infoLogLength, nullptr, infoLog.data());

          throw RuntimeErrorException(
            String::Format("Shader program linking failed: %s", infoLog.data())
          );
        }
        else
        {
          throw RuntimeErrorException("Shader program linking failed with no additional info.");
        }
      }
    }
    catch(...)
    {
      if (m_programId != 0)
      {
        glDeleteProgram(m_programId);
        m_programId = 0;
      }
      throw;
    }

    m_valid = true;
  }

  void OpenGlShaderProgram::bind()
  {
    assertIsValid();
    glUseProgram(m_programId);
  }

  bool OpenGlShaderProgram::isValid() const
  {
    return m_valid;
  }

  void OpenGlShaderProgram::setUniform(const String& name, float value)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, Int32 value)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, bool value)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform1i(location, value ? 1 : 0);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Vector3f& v3f)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform3f(location, v3f.x, v3f.y, v3f.z);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Color& color)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform4f(location, color.r, color.g, color.b, color.a);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Matrix4& matrix4x4)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix4x4.m[0]);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::setUniformTexture(const String& name, Int32 slotLocation)
  {
    assertIsValid();

    GLint location = getUniformLocation(name);
    glUniform1i(location, slotLocation);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }

  void OpenGlShaderProgram::destroy()
  {
    if (m_programId != 0)
    {
      glDeleteProgram(m_programId);
      m_programId = 0;
    }

    m_uniformLocationCache.clear();
    m_valid = false;
  }

  void OpenGlShaderProgram::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("Attempted to use an invalid shader program.");
  }

  GLint OpenGlShaderProgram::getUniformLocation(const String& name)
  {
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end())
      return it->second;

    GLint location = glGetUniformLocation(m_programId, name.c_str());
    if (location == -1)
      throw InvalidArgumentException(
        String::Format("Uniform '%s' not found in shader program.", name.c_str())
      );

    m_uniformLocationCache[name] = location;
    return location;
  }

  void OpenGlShaderProgram::attachShader(SharedPtr<IShader> shader)
  {
    if (!shader)
      throw InvalidArgumentException(
        "Attempted to attach a null shader to the shader program."
      );

    if (!shader->isValid())
      throw InvalidArgumentException(
        "Attempted to attach a shader that is not valid to the shader program."
      );

    auto glShader = std::dynamic_pointer_cast<OpenGlShader>(shader);
    if (!glShader)
      throw InvalidArgumentException(
        "Attempted to attach a shader that is not an OpenGlShader to the OpenGlShaderProgram."
      );

    GLuint shaderId = glShader->getShaderId();
    if (shaderId == 0)
      throw RuntimeErrorException(
        "Attempted to attach a shader with an invalid OpenGL shader ID (0) to the shader program."
      );

    glAttachShader(m_programId, shaderId);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
  }
}
