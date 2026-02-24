#pragma once

#include "hc/inputManager/hcIInputManager.h"
#include "hc/inputManager/hcKeyState.h"
#include "hc/inputManager/hcMouseState.h"

namespace hc
{
  class HotCoffeeEngine;

  class HC_CORE_EXPORT InputManager : public IInputManager
  {
  public:
    InputManager(HotCoffeeEngine&);
    ~InputManager() override = default;

    bool isKeyboardKeyPressed(keyboardKey::Type keyboardKey) override;
    bool isMouseButtonPressed(mouseButtonKey::Type mouseButtonKey) override;
    KeyState& getKeyboardKeyState(keyboardKey::Type keyboardKey) const;
    KeyState& getMouseButtonKeyState(mouseButtonKey::Type mouseButtonKey) const;
    MouseState& getMouseState() const;

  private:
    UnorderedMap<keyboardKey::Type, UniquePtr<KeyState>> m_keyboardKeyStates;
    UnorderedMap<mouseButtonKey::Type, UniquePtr<KeyState>> m_mouseButtonKeyStates;
    MouseState m_mouseState;
        
    bool onEvent(const Event& event) override;
    void destroy() override;
    
  };
}
