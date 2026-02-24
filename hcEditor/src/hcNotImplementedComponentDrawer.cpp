#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcNotImplementedComponentDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  componentType::Type NotImplementedComponentDrawer::getComponentType() const
  {
    return componentType::Unknown;
  }

  void NotImplementedComponentDrawer::drawComponent(IComponent* component)
  {
    if (!component)
      return;

    String componentStr = componentType::ToString(component->getType());
    String header = String::Format("%s component (View Not Implemented)", componentStr.c_str());
    if (ImGui::CollapsingHeader(header.c_str()))
    {
      ImGui::Text("Component View Not Implemented");
    }
  }
}
