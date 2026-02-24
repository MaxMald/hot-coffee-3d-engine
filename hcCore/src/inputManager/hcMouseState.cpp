#include "hc/inputManager/hcMouseState.h"

namespace hc
{
  MouseState::MouseState() :
    m_scrollState(),
    m_position(0, 0),
    m_previousPosition(0, 0)
  {
  }  

  Vector2i MouseState::getPosition() const
  {
    return m_position;
  }

  Vector2i MouseState::getPreviousPosition() const
  {
    return m_previousPosition;
  }

  Vector2i MouseState::getDeltaPosition() const
  {
    return Vector2i(
      m_position.x - m_previousPosition.x,
      m_position.y - m_previousPosition.y
    );
  }

  MouseScrollState& MouseState::getScrollState()
  {
    return m_scrollState;
  }

  void MouseState::setPosition(const Vector2i& position)
  {
    m_previousPosition = m_position;
    m_position = position;
  }

  void MouseState::resetScrollState()
  {
    m_scrollState.reset();
  }

  void MouseState::resetPreviousPosition()
  {
    m_previousPosition = m_position;
  }
}
