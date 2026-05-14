#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcALightComponentDrawer.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  ALightComponentDrawer::~ALightComponentDrawer()
  {}

  void ALightComponentDrawer::onDrawComponent(ALightComponent* component)
  {
    if (!component)
      return;

    component->updateLight();
    ALight& light = component->getLight();

    Color color = light.getColor();
    if (imguiUtilities::DrawColorEdit3("Color", color))
      light.setColor(color);

    float intensity = light.getIntensity();
    if (ImGui::InputFloat("Intensity", &intensity))
      light.setIntensity(intensity);

    bool enabled = light.isEnabled();
    if (ImGui::Checkbox("Enabled", &enabled))
      light.setEnabled(enabled);
  }

  ALightComponentDrawer::ALightComponentDrawer(componentType::Type type) :
    ABaseComponentDrawer<ALightComponent>(type)
  {}
}
