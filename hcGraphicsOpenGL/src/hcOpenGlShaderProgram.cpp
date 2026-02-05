#include "hc/hcOpenGlShaderProgram.h"
#include "hc/hcOpenGlShader.h"
#include "hc/hcOpenGlTexture.h"
#include "hc/hcOpenGlGraphicsUtilitites.h"

namespace hc
{
  OpenGlShaderProgram::OpenGlShaderProgram() :
    m_programId(glCreateProgram()),
    m_linked(false)
  {
  }

  OpenGlShaderProgram::~OpenGlShaderProgram()
  {
    destroy();
  }

  void OpenGlShaderProgram::bind()
  {
    if (m_linked)
      glUseProgram(m_programId);
  }

  bool OpenGlShaderProgram::isValid() const 
  {
    return m_linked;
  }

  void OpenGlShaderProgram::attachShader(SharedPtr<IShader> shader)
  {
    if (!shader || !shader->isCompiled())
    {
      LogService::Error("Attempted to attach an invalid or uncompiled shader.");
      return;
    }

    auto glShader = std::dynamic_pointer_cast<OpenGlShader>(shader);
    if (!glShader)
    {
      LogService::Error("Attempted to attach a non-OpenGL shader to OpenGL shader program.");
      return;
    }

    GLuint shaderId = glShader->getShaderId();
    if (shaderId != 0)
      glAttachShader(m_programId, shaderId);
  }

  void OpenGlShaderProgram::setUniform(const String& name, float value)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform1f(location, value);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, Int32 value)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform1i(location, value);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, bool value)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform1i(location, value ? 1 : 0);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Vector3f& v3f)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform3f(location, v3f.x, v3f.y, v3f.z);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Color& color)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform4f(location, color.r, color.g, color.b, color.a);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniform(const String& name, const Matrix4& matrix4x4)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniformMatrix4fv(location, 1, GL_TRUE, matrix4x4.m[0]);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::setUniformTexture(const String& name, Int32 slotLocation)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform1i(location, slotLocation);

    openGlGraphicsUtilities::checkAndLogPosibleError();
  }

  void OpenGlShaderProgram::destroy()
  {
    if (m_programId != 0)
    {
      glDeleteProgram(m_programId);
      m_programId = 0;
    }

    m_uniformLocationCache.clear();
    m_linked = false;
  }

  bool OpenGlShaderProgram::link()
  {
    glLinkProgram(m_programId);

    GLint linked = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &linked);
    if (!linked)
    {
      char log[1024];
      glGetProgramInfoLog(m_programId, sizeof(log), nullptr, log);
      LogService::Error(String("Program linking failed: ") + log);
      m_linked = false;
      return false;
    }

    m_linked = true;
    return true;
  }

  GLint OpenGlShaderProgram::getUniformLocation(const String& name)
  {
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end())
      return it->second;

    GLint location = glGetUniformLocation(m_programId, name.c_str());
    m_uniformLocationCache[name] = location;
    return location;
  }
}
