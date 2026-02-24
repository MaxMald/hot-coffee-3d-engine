#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IInputManager;

  class HC_CORE_EXPORT MouseScrollState : public NonCopyable
  {
  public:
    MouseScrollState();
    ~MouseScrollState() = default;
        
    float getHorizontalScrollDelta() const;
    float getVerticalScrollDelta() const;
    void reset();
    void setHorizontalScrollDelta(float horizontalScrollDelta);
    void setVerticalScrollDelta(float verticalScrollDelta);

  private:
    float m_horizontalScrollDelta;
    float m_verticalScrollDelta;

    friend class IInputManager;
  };
}
