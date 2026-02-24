#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/input/hcMouseWheelType.h"

namespace hc
{
  class MouseState;
  class InputManager;

  /**
   * @brief Represents the scroll state of the mouse wheel.
   * 
   * This class tracks the horizontal and vertical scroll deltas for
   * the mouse wheel. It is non-copyable and its state is managed
   * internally by the MouseState and InputManager classes.
   */
  class HC_CORE_EXPORT MouseScrollState : public NonCopyable
  {
  public:
    MouseScrollState();
    ~MouseScrollState() = default;

    /**
     * @brief Gets the horizontal scroll delta.
     *
     * @return The horizontal scroll delta value.
     */
    float getHorizontalScrollDelta() const;

    /**
     * @brief Gets the vertical scroll delta.
     *
     * @return The vertical scroll delta value.
     */
    float getVerticalScrollDelta() const;

  private:
    float m_horizontalScrollDelta;
    float m_verticalScrollDelta;
        
    void updateScrollDelta(mouseWheelType::Type wheelType, float value);
    void reset();

    friend class MouseState;
    friend class InputManager;
  };
}
