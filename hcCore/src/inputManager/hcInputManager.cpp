#include "hc/inputManager/hcInputManager.h"
#include "hc/hcHotCoffeeEngine.h"
#include "hc/window/input/hcEvent.h"

namespace hc
{
  InputManager::InputManager() :
    m_keyboardKeyStates(),
    m_mouseButtonKeyStates(),
    m_mouseState()
  {
  }

  bool InputManager::isKeyboardKeyPressed(
    keyboardKey::Type keyboardKey
  ) const
  {
    KeyState& keyState = getKeyboardKeyState(keyboardKey);
    return keyState.isPressed();
  }

  bool InputManager::isMouseButtonPressed(
    mouseButtonKey::Type mouseButtonKey
  ) const
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

  MouseState& InputManager::getMouseState()
  {
    return m_mouseState;
  }

  void InputManager::prepareForEventPolling()
  {
    m_mouseState.resetScrollState();
    m_mouseState.resetPreviousPosition();
  }

  bool InputManager::onEvent(const Event& event)
  {
    if (event.is<Event::KeyPressed>())
    {
      const auto& keyPressedEvent =
        event.getIf<Event::KeyPressed>();

      if (keyPressedEvent->keyCode == keyboardKey::Undefined)
        return false;

      getKeyboardKeyState(keyPressedEvent->keyCode).press();
    }
    else if (event.is<Event::KeyReleased>())
    {
      const auto& keyReleasedEvent =
        event.getIf<Event::KeyReleased>();

      if (keyReleasedEvent->keyCode == keyboardKey::Undefined)
        return false;

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
    else if (event.is<Event::MouseMoved>())
    {
      const auto& mouseMovedEvent =
        event.getIf<Event::MouseMoved>();
      m_mouseState.setPosition(mouseMovedEvent->position);
    }
    else if (event.is<Event::MouseWheelScrolled>())
    {
      const auto& mouseWheelScrolledEvent =
        event.getIf<Event::MouseWheelScrolled>();
      m_mouseState.getScrollState().updateScrollDelta(
        mouseWheelScrolledEvent->wheel,
        mouseWheelScrolledEvent->delta
      );
    }

    return false;
  }

  void InputManager::initialize()
  {
    Int8 keyboardKeyCount = static_cast<Int8>(keyboardKey::Type::Count);
    for (Int8 keyCode = 0; keyCode < keyboardKeyCount; ++keyCode)
      m_keyboardKeyStates[keyCode] = MakeUnique<KeyState>(keyCode);

    UInt8 mouseButtonCount = static_cast<UInt8>(mouseButtonKey::Type::Count);
    for (UInt8 mouseButtonCode = 0; mouseButtonCode < mouseButtonCount; ++mouseButtonCode)
    {
      m_mouseButtonKeyStates[mouseButtonCode] =
        MakeUnique<KeyState>(mouseButtonCode);
    }
  }

  void InputManager::destroy()
  {
    m_keyboardKeyStates.clear();
    m_mouseButtonKeyStates.clear();
  }
}
