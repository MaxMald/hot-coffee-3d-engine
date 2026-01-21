#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  namespace imguiUtilities
  {
    bool DrawColorEdit3(const String& label, Color& color)
    {
      float col[3] = { color.r, color.g, color.b };
      if(ImGui::ColorEdit3(label.c_str(), col))
      {
        color.r = col[0];
        color.g = col[1];
        color.b = col[2];
        return true;
      }
      return false;
    }
  }
}
