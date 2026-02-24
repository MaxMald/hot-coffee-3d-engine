#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/input/hcKeyboardKey.h"

namespace hc
{
  class IInputManager;

  class HC_CORE_EXPORT KeyState
  {
  public:
    KeyState(UInt8 keyCode);
    ~KeyState() = default;

    UInt8 getKeyCode() const;
    bool isPressed() const;
    void press();
    void release();

  private:
    UInt8 m_keyCode;
    bool m_isPressed;
  };
}
