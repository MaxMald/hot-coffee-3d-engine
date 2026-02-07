#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  namespace imguiUtilities
  {
    /**
     * @brief Draws a 3-component color editor widget in ImGui.
     *
     * Allows the user to edit the RGB values of the provided color.
     * 
     * @param label The label to display next to the widget.
     * @param color Reference to the color to be edited.
     * 
     * @return True if the color was modified, false otherwise.
     */
    bool DrawColorEdit3(const String& label, Color& color);

    /**
     * @brief Draws a read-only color display widget in ImGui.
     *
     * Shows the color as a block and its RGBA values for reference.
     *
     * @param label The label to display next to the widget.
     * @param color The color to display.
     */
    void DrawColor(const String& label, const Color& color);

    /**
     * @brief Draws a matrix display widget in ImGui.
     *
     * Shows the contents of a 4x4 matrix for inspection.
     * 
     * @param label The label to display next to the widget.
     * @param matrix The matrix to display.
     */
    void DrawMatrix(const String& label, const Matrix4& matrix);
  }
}

