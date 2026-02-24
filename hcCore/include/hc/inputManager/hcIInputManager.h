#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIEventListener.h"
#include "hc/window/input/hcKeyboardKey.h"
#include "hc/window/input/hcMouseButtonKey.h"

namespace hc
{
  class HotCoffeeEngine;
  class KeyState;
  class MouseState;

  class HC_CORE_EXPORT IInputManager :
    public IEventListener
  {
  public:
    virtual ~IInputManager() = default;

    virtual bool isKeyboardKeyPressed(keyboardKey::Type keyboardKey) = 0;
    virtual bool isMouseButtonPressed(mouseButtonKey::Type mouseButtonKey) = 0;
    virtual KeyState& getKeyboardKeyState(keyboardKey::Type keyboardKey) const = 0;
    virtual KeyState& getMouseButtonKeyState(mouseButtonKey::Type mouseButtonKey) = 0;
    virtual MouseState& getMouseState() const = 0;

  protected:
    IInputManager() = default;

    virtual void reset() = 0;
    virtual void destroy() = 0;    

    friend class HotCoffeeEngine;
  };
}
