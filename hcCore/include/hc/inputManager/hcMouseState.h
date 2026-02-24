#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/inputManager/hcMouseScrollState.h"

namespace hc
{
  class InputManager;

  /**
   * @brief Represents the state of the mouse including position and scroll state.
   * 
   * This class tracks the current mouse position and scroll state. It is non-copyable
   * and its state is managed internally by the InputManager class.
   */
  class HC_CORE_EXPORT MouseState : public NonCopyable
  {
  public:
    /**
     * @brief Constructs a MouseState object with default values.
     */
    MouseState();

    /**
     * @brief Default destructor.
     */
    ~MouseState() = default;

    /**
     * @brief Gets the current mouse position.
     *
     * @return The mouse position as a 2D integer vector.
     */
    Vector2i getPosition() const;

    /**
     * @brief Gets the previous mouse position.
     *
     * @return The previous mouse position as a 2D integer vector.
     */
    Vector2i getPreviousPosition() const;

    /**
     * @brief Gets the change in mouse position since the last update.
     *
     * @return The delta position as a 2D integer vector.
     */
    Vector2i getDeltaPosition() const;

    /**
     * @brief Gets the scroll state of the mouse.
     *
     * @return Reference to the MouseScrollState object.
     */
    MouseScrollState& getScrollState();

  private:
    MouseScrollState m_scrollState;
    Vector2i m_position;
    Vector2i m_previousPosition;

    /**
     * @brief Sets the mouse position.
     *
     * @param position The new mouse position.
     */
    void setPosition(const Vector2i& position);

    /**
     * @brief Resets the scroll state to default values.
     */
    void resetScrollState();

    /**
     * @brief Resets the previous mouse position to the current position.
     */
    void resetPreviousPosition();

    friend class InputManager;
  };
}
