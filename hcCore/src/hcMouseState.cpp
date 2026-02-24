#include "hc/inputManager/hcMouseState.h"

namespace hc
{
  MouseState::MouseState() :
    m_scrollState(),
    m_position(0.0f, 0.0f)
  {
  }

  void MouseState::reset()
  {
    m_scrollState.reset();
  }

  Vector2i MouseState::getPosition() const
  {
    return m_position;
  }

  MouseScrollState& MouseState::getScrollState()
  {
    return m_scrollState;
  }

  void MouseState::setPosition(const Vector2i& position)
  {
    m_position = position;
  }
}
