#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  namespace imguiUtilities
  {
    /**
     * @brief Draws an input text widget in ImGui.
     *
     * Allows the user to edit a string value with a label.
     *
     * @param label The label to display next to the input text widget.
     * @param text Reference to the string that will be edited by the user.
     *
     * @return True if the text was modified by the user, false otherwise.
     */
    bool DrawInputText(const String& label, String& text);

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

    /**
     * @brief Draws a texture in ImGui.
     *
     * Displays the given texture at the specified dimensions.
     *
     * @param texture Pointer to the texture to be drawn.
     * @param width The width to display the texture.
     * @param height The height to display the texture.
     */
    void DrawTexture(
      ITexture* texture,
      float width,
      float height
    );
  }
}

