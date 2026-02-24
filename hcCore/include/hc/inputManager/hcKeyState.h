#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/input/hcKeyboardKey.h"

namespace hc
{
  class InputManager;

  /**
   * @brief Represents the state of an input key/button.
   * 
   * This class tracks whether a specific input key/button is currently pressed or released.
   * The state is managed internally by the InputManager class.
   */
  class HC_CORE_EXPORT KeyState
  {
  public:
    /**
     * @brief Constructs a KeyState object for the specified input key/button.
     * 
     * @param keyCode The code of the input key/button to track.
     */
    KeyState(UInt8 keyCode);
    ~KeyState() = default;

    /**
     * @brief Gets the key code associated with this state.
     * 
     * @return The key code as an unsigned 8-bit integer.
     */
    UInt8 getKeyCode() const;

    /**
     * @brief Checks if the input key/button is currently pressed.
     * 
     * @return true if the input key/button is pressed, false otherwise.
     */
    bool isPressed() const;

  private:
    UInt8 m_keyCode;
    bool m_isPressed;

    /**
     * @brief Marks the key as pressed. 
     */
    void press();

    /**
     * @brief Marks the key as released.
     */
    void release();

    friend class InputManager;
  };
}
