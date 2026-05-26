#include "hc/graphics/ubos/hcLightFrameUBO.h"

#include <GL/glew.h>
#include <hc/graphics/lightFrameData/hcLightFrameData.h>

namespace hc
{
  LightFrameUBO::LightFrameUBO() :
    m_ubo(0),
    m_isInitialized(false)
  {}

  LightFrameUBO::~LightFrameUBO()
  {
    destroy();
  }

  void LightFrameUBO::initialize()
  {
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightFrameData), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_isInitialized = true;
  }

  void LightFrameUBO::bind(UInt32 bindingIndex)
  {
    assertIsInitialized();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, m_ubo);
  }

  void LightFrameUBO::upload(const LightFrameData& lightFrameData)
  {
    assertIsInitialized();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightFrameData), &lightFrameData);
  }

  void LightFrameUBO::unbind()
  {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void LightFrameUBO::destroy()
  {
    glDeleteBuffers(1, &m_ubo);
    m_ubo = 0;
    m_isInitialized = false;
  }

  bool LightFrameUBO::isInitialized() const
  {
    return m_isInitialized;
  }
}
