#include "hc/hcOpenGlShaderProgram.h"
#include <hc/hcLogService.h>
#include "hc/hcOpenGlShader.h"

namespace hc
{
  OpenGlShaderProgram::OpenGlShaderProgram(const String& key) :
    m_programId(glCreateProgram()),
    m_key(key),
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

  const String& OpenGlShaderProgram::getKey() const
  {
    return m_key;
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
  }

  void OpenGlShaderProgram::setUniform(const String& name, Int32 value)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniform1i(location, value);
  }

  void OpenGlShaderProgram::setUniform(const String& name, const float* matrix4x4)
  {
    GLint location = getUniformLocation(name);
    if (location != -1)
      glUniformMatrix4fv(location, 1, GL_FALSE, matrix4x4);
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
}
