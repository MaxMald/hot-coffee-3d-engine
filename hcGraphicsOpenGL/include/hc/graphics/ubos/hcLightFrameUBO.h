#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class LightFrameUBO
  {
  public:
    LightFrameUBO();
    ~LightFrameUBO();

    void initialize();
    void bind(UInt32 bindingIndex);
    void upload(const LightFrameData& lightFrameData);
    void unbind();
    void destroy();
    bool isInitialized() const;

  private:
    UInt32 m_ubo;
    bool m_isInitialized;

    void assertIsInitialized() const
    {
      if (!m_isInitialized)
        throw RuntimeErrorException("LightFrameUBO is not initialized.");
    }
  };
}
