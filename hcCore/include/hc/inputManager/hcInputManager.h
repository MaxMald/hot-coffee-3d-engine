#pragma once

#include "hc/window/input/hcEvent.h"
#include "hc/window/input/hcKeyboardKey.h"
#include "hc/window/input/hcMouseButtonKey.h"
#include "hc/inputManager/hcKeyState.h"
#include "hc/inputManager/hcMouseState.h"

namespace hc
{
  class HotCoffeeEngine;

  /**
   * @brief Manages input from keyboard and mouse devices.
   *
   * The InputManager tracks the state of keyboard keys and mouse buttons,
   * providing an interface to query their current pressed/released status.
   * It listens to input events from the engine and updates the internal state accordingly.
   */
  class HC_CORE_EXPORT InputManager
  {
  public:
    InputManager();
    ~InputManager() = default;

    /**
     * @brief Check if a keyboard key is currently pressed.
     *
     * @param keyboardKey The keyboard key to check.
     *
     * @return True if the key is pressed, false otherwise.
     */
    bool isKeyboardKeyPressed(keyboardKey::Type keyboardKey) const;

    /**
     * @brief Check if a mouse button is currently pressed.
     *
     * @param mouseButtonKey The mouse button to check.
     *
     * @return True if the button is pressed, false otherwise.
     */
    bool isMouseButtonPressed(mouseButtonKey::Type mouseButtonKey) const;

    /**
     * @brief Get the state object for a specific keyboard key.
     *
     * @param keyboardKey The keyboard key to get the state for.
     *
     * @return Reference to the KeyState object tracking the key.
     */
    KeyState& getKeyboardKeyState(keyboardKey::Type keyboardKey) const;

    /**
     * @brief Get the state object for a specific mouse button.
     *
     * @param mouseButtonKey The mouse button to get the state for.
     *
     * @return Reference to the KeyState object tracking the button.
     */
    KeyState& getMouseButtonKeyState(mouseButtonKey::Type mouseButtonKey) const;

    /**
     * @brief Get the current mouse state.
     *
     * @return Reference to the MouseState object containing position and other mouse data.
     */
    MouseState& getMouseState();

  private:
    UnorderedMap<Int8, UniquePtr<KeyState>> m_keyboardKeyStates;
    UnorderedMap<UInt8, UniquePtr<KeyState>> m_mouseButtonKeyStates;
    MouseState m_mouseState;

    /**
    * @brief Prepare the InputManager for a new frame of event polling.
    */
    void prepareForEventPolling();

    /**
     * @brief Handle incoming events and update input states.
     *
     * @param event The event to process.
     *
     * @return True if the event was handled, false otherwise.
     */
    bool onEvent(const Event& event);

    /**
     * @brief Initialize the InputManager.
     */
    void initialize();

    /**
     * @brief Clean up and destroy the InputManager.
     */
    void destroy();

    friend class HotCoffeeEngine;
  };
}
