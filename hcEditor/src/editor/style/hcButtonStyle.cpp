#include "hc/editor/style/hcButtonStyle.h"

namespace hc::editor::style::buttons
{
  ButtonStyle ButtonStyle::GetStyle(Type buttonType)
  {
    switch (buttonType)
    {
    case AggressiveRed:
      return ButtonStyle(
        Color(0.9f, 0.1f, 0.1f, 1.0f),
        Color(1.0f, 0.2f, 0.2f, 1.0f),
        Color(0.75f, 0.05f, 0.05f, 1.0f),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
      );
    default:
      return ButtonStyle(); // Default style
    }
  }
}
