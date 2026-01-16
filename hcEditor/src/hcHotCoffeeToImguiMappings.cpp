#include "hc/editor/hcHotCoffeeToImguiMappings.h"

namespace hc::editor
{
  namespace hotCoffeeToImgui
  {
    ImGuiMouseButton_ mapMouseButton(const mouseButtonKey::Type& button)
    {
      switch (button)
      {
      case mouseButtonKey::Left:
        return ImGuiMouseButton_Left;
      case mouseButtonKey::Right:
        return ImGuiMouseButton_Right;
      case mouseButtonKey::Middle:
        return ImGuiMouseButton_Middle;
      default:
        return ImGuiMouseButton_COUNT; // Invalid button
      }
    }

    ImGuiKey mapKeyboardCode(const keyboardKey::Type& key)
    {
      switch (key)
      {
      case keyboardKey::A: return ImGuiKey_A;
      case keyboardKey::B: return ImGuiKey_B;
      case keyboardKey::C: return ImGuiKey_C;
      case keyboardKey::D: return ImGuiKey_D;
      case keyboardKey::E: return ImGuiKey_E;
      case keyboardKey::F: return ImGuiKey_F;
      case keyboardKey::G: return ImGuiKey_G;
      case keyboardKey::H: return ImGuiKey_H;
      case keyboardKey::I: return ImGuiKey_I;
      case keyboardKey::J: return ImGuiKey_J;
      case keyboardKey::K: return ImGuiKey_K;
      case keyboardKey::L: return ImGuiKey_L;
      case keyboardKey::M: return ImGuiKey_M;
      case keyboardKey::N: return ImGuiKey_N;
      case keyboardKey::O: return ImGuiKey_O;
      case keyboardKey::P: return ImGuiKey_P;
      case keyboardKey::Q: return ImGuiKey_Q;
      case keyboardKey::R: return ImGuiKey_R;
      case keyboardKey::S: return ImGuiKey_S;
      case keyboardKey::T: return ImGuiKey_T;
      case keyboardKey::U: return ImGuiKey_U;
      case keyboardKey::V: return ImGuiKey_V;
      case keyboardKey::W: return ImGuiKey_W;
      case keyboardKey::X: return ImGuiKey_X;
      case keyboardKey::Y: return ImGuiKey_Y;
      case keyboardKey::Z: return ImGuiKey_Z;
      case keyboardKey::Num0: return ImGuiKey_0;
      case keyboardKey::Num1: return ImGuiKey_1;
      case keyboardKey::Num2: return ImGuiKey_2;
      case keyboardKey::Num3: return ImGuiKey_3;
      case keyboardKey::Num4: return ImGuiKey_4;
      case keyboardKey::Num5: return ImGuiKey_5;
      case keyboardKey::Num6: return ImGuiKey_6;
      case keyboardKey::Num7: return ImGuiKey_7;
      case keyboardKey::Num8: return ImGuiKey_8;
      case keyboardKey::Num9: return ImGuiKey_9;
      case keyboardKey::Escape: return ImGuiKey_Escape;
      case keyboardKey::LControl: return ImGuiKey_LeftCtrl;
      case keyboardKey::LShift: return ImGuiKey_LeftShift;
      case keyboardKey::LAlt: return ImGuiKey_LeftAlt;
      case keyboardKey::LSystem: return ImGuiKey_LeftSuper;
      case keyboardKey::RControl: return ImGuiKey_RightCtrl;
      case keyboardKey::RShift: return ImGuiKey_RightShift;
      case keyboardKey::RAlt: return ImGuiKey_RightAlt;
      case keyboardKey::RSystem: return ImGuiKey_RightSuper;
      case keyboardKey::Menu: return ImGuiKey_Menu;
      case keyboardKey::LBracket: return ImGuiKey_LeftBracket;
      case keyboardKey::RBracket: return ImGuiKey_RightBracket;
      case keyboardKey::SemiColon: return ImGuiKey_Semicolon;
      case keyboardKey::Comma: return ImGuiKey_Comma;
      case keyboardKey::Period: return ImGuiKey_Period;
      case keyboardKey::Quote: return ImGuiKey_Apostrophe;
      case keyboardKey::Slash: return ImGuiKey_Slash;
      case keyboardKey::BackSlash: return ImGuiKey_Backslash;
      case keyboardKey::Tilde: return ImGuiKey_GraveAccent;
      case keyboardKey::Equal: return ImGuiKey_Equal;
      case keyboardKey::Dash: return ImGuiKey_Minus;
      case keyboardKey::Space: return ImGuiKey_Space;
      case keyboardKey::Return: return ImGuiKey_Enter;
      case keyboardKey::BackSpace: return ImGuiKey_Backspace;
      case keyboardKey::Tab: return ImGuiKey_Tab;
      case keyboardKey::PageUp: return ImGuiKey_PageUp;
      case keyboardKey::PageDown: return ImGuiKey_PageDown;
      case keyboardKey::End: return ImGuiKey_End;
      case keyboardKey::Home: return ImGuiKey_Home;
      case keyboardKey::Insert: return ImGuiKey_Insert;
      case keyboardKey::Delete: return ImGuiKey_Delete;
      case keyboardKey::Add: return ImGuiKey_KeypadAdd;
      case keyboardKey::Subtract: return ImGuiKey_KeypadSubtract;
      case keyboardKey::Multiply: return ImGuiKey_KeypadMultiply;
      case keyboardKey::Divide: return ImGuiKey_KeypadDivide;
      case keyboardKey::Left: return ImGuiKey_LeftArrow;
      case keyboardKey::Right: return ImGuiKey_RightArrow;
      case keyboardKey::Up: return ImGuiKey_UpArrow;
      case keyboardKey::Down: return ImGuiKey_DownArrow;
      case keyboardKey::Numpad0: return ImGuiKey_Keypad0;
      case keyboardKey::Numpad1: return ImGuiKey_Keypad1;
      case keyboardKey::Numpad2: return ImGuiKey_Keypad2;
      case keyboardKey::Numpad3: return ImGuiKey_Keypad3;
      case keyboardKey::Numpad4: return ImGuiKey_Keypad4;
      case keyboardKey::Numpad5: return ImGuiKey_Keypad5;
      case keyboardKey::Numpad6: return ImGuiKey_Keypad6;
      case keyboardKey::Numpad7: return ImGuiKey_Keypad7;
      case keyboardKey::Numpad8: return ImGuiKey_Keypad8;
      case keyboardKey::Numpad9: return ImGuiKey_Keypad9;
      case keyboardKey::F1: return ImGuiKey_F1;
      case keyboardKey::F2: return ImGuiKey_F2;
      case keyboardKey::F3: return ImGuiKey_F3;
      case keyboardKey::F4: return ImGuiKey_F4;
      case keyboardKey::F5: return ImGuiKey_F5;
      case keyboardKey::F6: return ImGuiKey_F6;
      case keyboardKey::F7: return ImGuiKey_F7;
      case keyboardKey::F8: return ImGuiKey_F8;
      case keyboardKey::F9: return ImGuiKey_F9;
      case keyboardKey::F10: return ImGuiKey_F10;
      case keyboardKey::F11: return ImGuiKey_F11;
      case keyboardKey::F12: return ImGuiKey_F12;
      case keyboardKey::Pause: return ImGuiKey_Pause;
      default: return ImGuiKey_None;
      }
    }
  }
}
