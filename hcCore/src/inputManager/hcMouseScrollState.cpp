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

  void MouseScrollState::updateScrollDelta(
    mouseWheelType::Type wheelType,
    float value
  )
  {
    if (wheelType == mouseWheelType::Horizontal)
      m_horizontalScrollDelta = value;
    else if (wheelType == mouseWheelType::Vertical)
      m_verticalScrollDelta = value;
    else
      throw InvalidArgumentException(
        "Invalid mouse wheel type: " + std::to_string(wheelType)
      );
  }

  void MouseScrollState::reset()
  {
    m_horizontalScrollDelta = 0.0f;
    m_verticalScrollDelta = 0.0f;
  }
}
