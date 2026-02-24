#include "hc/inputManager/hcMouseState.h"

namespace hc
{
  MouseState::MouseState() :
    m_scrollState(),
    m_position(0, 0)
  {
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

  void MouseState::resetScrollState()
  {
    m_scrollState.reset();
  }
}
