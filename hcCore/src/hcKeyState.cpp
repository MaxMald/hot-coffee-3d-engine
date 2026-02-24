#include "hcKeyState.h"
#include "hcKeyState.h"
#include "hc/inputManager/hcKeyState.h"

namespace hc
{
  KeyState::KeyState(UInt8 keyCode) :
    m_keyCode(keyCode),
    m_isPressed(false)
  {
  }

  UInt8 KeyState::getKeyCode() const
  {
    return m_keyCode;
  }

  bool KeyState::isPressed() const
  {
    return m_isPressed;
  }

  void KeyState::press()
  {
    m_isPressed = true;
  }

  void KeyState::release()
  {
    m_isPressed = false;
  }
}
