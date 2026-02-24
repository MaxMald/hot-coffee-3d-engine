#include "hc/inputManager/hcInputManager.h"
#include "hc/hcHotCoffeeEngine.h"
#include "hc/window/input/hcEvent.h"

namespace hc
{
  InputManager::InputManager(HotCoffeeEngine& engine) :
    m_keyboardKeyStates(),
    m_mouseButtonKeyStates(),
    m_mouseState()
  {
    engine.addEventListener(this);
  }

  bool InputManager::isKeyboardKeyPressed(keyboardKey::Type keyboardKey)
  {
    KeyState& keyState = getKeyboardKeyState(keyboardKey);
    return keyState.isPressed();
  }

  bool InputManager::isMouseButtonPressed(mouseButtonKey::Type mouseButtonKey)
  {
    KeyState& keyState = getMouseButtonKeyState(mouseButtonKey);
    return keyState.isPressed();
  }

  KeyState& hc::InputManager::getKeyboardKeyState(
    keyboardKey::Type keyboardKey
  ) const
  {
    auto it = m_keyboardKeyStates.find(keyboardKey);
    if (it != m_keyboardKeyStates.end())
      return *(it->second);

    throw RuntimeErrorException(
      "Key state for keyboard key with code " + std::to_string(keyboardKey) + " not found."
    );
  }

  KeyState& hc::InputManager::getMouseButtonKeyState(
    mouseButtonKey::Type mouseButtonKey
  ) const
  {
    auto it = m_mouseButtonKeyStates.find(mouseButtonKey);
    if (it != m_mouseButtonKeyStates.end())
      return *(it->second);

    throw RuntimeErrorException(
      "Key state for mouse button key with code " + std::to_string(mouseButtonKey) + " not found."
    );
  }

  MouseState& InputManager::getMouseState() const
  {
    return m_mouseState;
  }

  bool InputManager::onEvent(const Event& event)
  {
    if (event.is<Event::KeyPressed>())
    {
      const auto& keyPressedEvent =
        event.getIf<Event::KeyPressed>();
      getKeyboardKeyState(keyPressedEvent->keyCode).press();
    }
    else if (event.is<Event::KeyReleased>())
    {
      const auto& keyReleasedEvent =
        event.getIf<Event::KeyReleased>();
      getKeyboardKeyState(keyReleasedEvent->keyCode).release();
    }
    else if (event.is<Event::MouseButtonPressed>())
    {
      const auto& mouseButtonPressedEvent =
        event.getIf<Event::MouseButtonPressed>();
      getMouseButtonKeyState(mouseButtonPressedEvent->button).press();
    }
    else if (event.is<Event::MouseButtonReleased>())
    {
      const auto& mouseButtonReleasedEvent =
        event.getIf<Event::MouseButtonReleased>();
      getMouseButtonKeyState(mouseButtonReleasedEvent->button).release();
    }

    return false;
  }

  void InputManager::destroy()
  {
    m_keyboardKeyStates.clear();
    m_mouseButtonKeyStates.clear();
    engine.removeEventListener(this);
  }
}
