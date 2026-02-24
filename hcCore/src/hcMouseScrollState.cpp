#include "hc/inputManager/hcMouseScrollState.h"

namespace hc
{
  MouseScrollState::MouseScrollState() :
    m_horizontalScrollDelta(0.0f),
    m_verticalScrollDelta(0.0f)
  {
  }

  float MouseScrollState::getHorizontalScrollDelta() const
  {
    return m_horizontalScrollDelta;
  }

  float MouseScrollState::getVerticalScrollDelta() const
  {
    return m_verticalScrollDelta;
  }

  void MouseScrollState::reset()
  {
    m_horizontalScrollDelta = 0.0f;
    m_verticalScrollDelta = 0.0f;
  }

  void MouseScrollState::setHorizontalScrollDelta(float horizontalScrollDelta)
  {
    m_horizontalScrollDelta = horizontalScrollDelta;
  }

  void MouseScrollState::setVerticalScrollDelta(float verticalScrollDelta)
  {
    m_verticalScrollDelta = verticalScrollDelta;
  }
}
