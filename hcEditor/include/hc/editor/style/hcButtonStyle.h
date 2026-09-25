#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor::style::buttons
{
  /**
   * @brief Types of buttons in the editor.
   */
  enum Type : UInt32
  {
    AggressiveRed = 0
  };

  /**
   * @brief Defines the style for buttons in the editor.
   */
  struct ButtonStyle
  {
    Color normalColor = Color(0.2f, 0.6f, 0.4f, 1.0f);  ///< Normal state color of the button.
    Color hoverColor = Color(0.3f, 0.7f, 0.5f, 1.0f);   ///< Hovered state color of the button.
    Color activeColor = Color(0.1f, 0.5f, 0.3f, 1.0f);  ///< Clicked state color of the button.
    Color textColor = Color(1.0f, 1.0f, 1.0f, 1.0f);    ///< Text color of the button.

    static ButtonStyle GetStyle(Type buttonType);

    /**
     * @brief Default constructor for ButtonStyle.
     */
    ButtonStyle() = default;

    /**
     * @brief Constructs a ButtonStyle with specified colors.
     * @param normal The color for the normal state.
     * @param hover The color for the hovered state.
     * @param active The color for the clicked state.
     * @param text The color for the button text.
     */
    ButtonStyle(const Color normal, const Color hover, const Color active, const Color text)
      : normalColor(normal), hoverColor(hover), activeColor(active), textColor(text) {}
  };
}
