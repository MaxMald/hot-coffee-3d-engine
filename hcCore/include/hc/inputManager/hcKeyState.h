#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/input/hcKeyboardKey.h"

namespace hc
{
  class InputManager;

  /**
   * @brief Represents the state of a keyboard key.
   * 
   * This class tracks whether a specific keyboard key is currently pressed or released.
   * The state is managed internally by the InputManager class.
   */
  class HC_CORE_EXPORT KeyState
  {
  public:
    /**
     * @brief Constructs a KeyState object for the specified key.
     * 
     * @param keyCode The code of the keyboard key to track.
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
     * @brief Checks if the key is currently pressed.
     * 
     * @return true if the key is pressed, false otherwise.
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
