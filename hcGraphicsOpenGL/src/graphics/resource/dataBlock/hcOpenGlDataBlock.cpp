#include "hc/graphics/resource/dataBlock/hcOpenGlDataBlock.h"

#include <GL/glew.h>

#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlDataBlock::OpenGlDataBlock() :
    m_id(Id::Create()),
    m_ubo(0),
    m_dataSize(0),
    m_isInitialized(false)
  {
  }

  OpenGlDataBlock::~OpenGlDataBlock()
  {
    destroy();
  }

  const Id& OpenGlDataBlock::getId() const
  {
    return m_id;
  }

  bool OpenGlDataBlock::isValid() const
  {
    return m_isInitialized;
  }

  void OpenGlDataBlock::destroy()
  {
    if (m_ubo != 0)
    {
      glDeleteBuffers(1, &m_ubo);
      m_ubo = 0;
    }
    m_isInitialized = false;
  }

  void OpenGlDataBlock::initialize(const void* data, SizeT dataSize)
  {
    if (m_isInitialized)
      throw RuntimeErrorException("UniformBufferObject is already initialized.");

    GLint currentUBOBinding = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentUBOBinding);

    try
    {
      glGenBuffers(1, &m_ubo);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      destroy();
      glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
      throw;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
    m_isInitialized = true;
    m_dataSize = dataSize;
  }

  void OpenGlDataBlock::upload(const void* data)
  {
    assertIsValid();

    GLint currentUBOBinding = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentUBOBinding);

    try
    {
      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, m_dataSize, data);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
      throw;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
  }

  void OpenGlDataBlock::bind(UInt32 bindingIndex)
  {
    assertIsValid();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, m_ubo);
  }

  void  OpenGlDataBlock::assertIsValid() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("UniformBufferObject is not initialized.");
  }
}
