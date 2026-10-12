#pragma once

#include <imgui.h>

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/style/hcStyle.h"


namespace hc::editor
{
  namespace hcImGui
  {
    /**
     * @brief Converts a Color object to an ImVec4 object for ImGui.
     * @param color The Color object to convert.
     * @return An ImVec4 object representing the same color.
     */
    inline ImVec4 ColorToImVec4(const Color& color)
    {
      return ImVec4(color.r, color.g, color.b, color.a);
    }

    /**
     * @brief Converts a Vector2u object to an ImVec2 object for ImGui.
     * @param vec The Vector2u object to convert.
     * @return An ImVec2 object representing the same 2D vector.
     */
    inline ImVec2 Vector2uToImVec2(const Vector2u& vec)
    {
      return ImVec2(static_cast<float>(vec.x), static_cast<float>(vec.y));
    }

    /**
     * @brief Displays the filename extracted from the provided path, and when hovered, it
     * shows a tooltip with the full path.
     *
     * @param label The label to display next to the filename.
     * @param path The Path object from which to extract the filename.
     */
    inline void Filename(const String& label, const Path& path)
    {
      if (path.empty())
      {
        ImGui::Text("%s: (No file)", label.c_str());
        return;
      }

      String fileName = path.filename().toString();
      ImGui::Text("%s: %s", label.c_str(), fileName.c_str());
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s", path.toString().c_str());
    }

    /**
     * @brief Draws a button in ImGui with the specified text and style.
     *
     * @param text The text to display on the button.
     * @param type The style type of the button (default is Default).
     * @param size_arg The size of the button (default is Vector2u(0, 0) for automatic
     * sizing).
     *
     * @return True if the button was clicked, false otherwise.
     */
    inline bool Button(
      const String& text,
      const style::buttons::Type type = style::buttons::Type::Default,
      const Vector2u& size_arg = Vector2u(0, 0)
    )
    {
      const style::buttons::ButtonStyle& buttonStyle = style::buttons::ButtonStyle::GetStyle(type);
      ImGui::PushStyleColor(ImGuiCol_Button, ColorToImVec4(buttonStyle.normalColor));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ColorToImVec4(buttonStyle.hoverColor));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ColorToImVec4(buttonStyle.activeColor));
      ImGui::PushStyleColor(ImGuiCol_Text, ColorToImVec4(buttonStyle.textColor));

      bool clicked = ImGui::Button(text.c_str(), Vector2uToImVec2(size_arg));
      ImGui::PopStyleColor(4);
      return clicked;
    }

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
      const ITexture* texture,
      float width,
      float height
    );

    /**
     * @brief Draws a texture in ImGui with specified UV coordinates.
     *
     * Displays the given texture at the specified dimensions, using the provided UV
     * coordinates to determine which part of the texture to display.
     *
     * @param texture Pointer to the texture to be drawn.
     * @param width The width to display the texture.
     * @param height The height to display the texture.
     * @param uvTopLeft The UV coordinates of the top-left corner of the texture region
     * to display.
     * @param uvBottomRight The UV coordinates of the bottom-right corner of the texture
     * region to display.
     */
    void DrawTexture(
      const ITexture* texture,
      float width,
      float height,
      const Vector2f& uvTopLeft,
      const Vector2f& uvBottomRight
    );
  }
}

