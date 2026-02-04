#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  namespace imguiUtilities
  {
    bool DrawColorEdit3(const String& label, Color& color)
    {
      bool changed = false;

      ImGui::BeginTable("LabelValueTable", 2, ImGuiTableFlags_SizingFixedFit);
      ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 200.0f);
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text(label.c_str());
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
  }
}
