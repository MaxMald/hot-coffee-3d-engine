#include "hc/editor/imgui/hcImguiUtilities.h"
#include "imgui.h"

#include <cstdio>

namespace hc::editor
{
  namespace imguiUtilities
  {
    bool DrawInputText(const String& label, String& text)
    {
      char nameBuffer[256];
      std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", text.c_str());

      if (ImGui::InputText(label.c_str(), nameBuffer, sizeof(nameBuffer)))
      {
        text = nameBuffer;
        return true;
      }

      return false;
    }

    bool DrawColorEdit3(const String& label, Color& color)
    {
      bool changed = false;

      ImGui::BeginTable("LabelValueTable", 2, ImGuiTableFlags_SizingFixedFit);
      ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 200.0f);
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%s", label.c_str());
      ImGui::TableSetColumnIndex(1);

      ImGui::SetNextItemWidth(200.0f);
      float col[3] = { color.r, color.g, color.b };
      if (ImGui::ColorEdit3("##Color", col))
      {
        color.r = col[0];
        color.g = col[1];
        color.b = col[2];
        changed = true;
      }

      ImGui::EndTable();
      return changed;
    }

    void DrawColor(const String& label, const Color& color)
    {
      ImVec4 colorVec(
        static_cast<float>(color.r),
        static_cast<float>(color.g),
        static_cast<float>(color.b),
        static_cast<float>(color.a)
      );

      ImGui::Text("%s", label.c_str());
      ImGui::SameLine();
      ImGui::ColorButton("##MaterialColor", colorVec, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker, ImVec2(32, 32));
      ImGui::SameLine();
      ImGui::Text("RGBA: %.3f, %.3f, %.3f, %.3f", colorVec.x, colorVec.y, colorVec.z, colorVec.w);
    }

    void DrawMatrix(const String& label, const Matrix4& matrix)
    {
      ImGui::BeginTable(label.c_str(), 4, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit);
      for (int row = 0; row < 4; ++row)
      {
        ImGui::TableNextRow();
        for (int col = 0; col < 4; ++col)
        {
          ImGui::TableSetColumnIndex(col);
          float value = matrix.m[row][col];
          ImGui::Text("%.3f", value);
        }
      }
      ImGui::EndTable();
    }

    void DrawTexture(
      const ITexture* texture,
      float width,
      float height
    )
    {
      if (texture)
      {
        ImGui::Image(
          texture->getNativeHandle(),
          ImVec2(width, height)
        );
      }
      else
      {
        ImGui::TextDisabled("[No Texture]");
      }
    }
    void DrawTexture(
      const ITexture* texture,
      float width,
      float height,
      const Vector2f& uvTopLeft,
      const Vector2f& uvBottomRight
    )
    {
      if (texture)
      {
        ImGui::Image(
          texture->getNativeHandle(),
          ImVec2(width, height),
          ImVec2(uvTopLeft.x, uvTopLeft.y),
          ImVec2(uvBottomRight.x, uvBottomRight.y)
        );
      }
      else
      {
        ImGui::TextDisabled("[No Texture]");
      }
    }
  }
}
