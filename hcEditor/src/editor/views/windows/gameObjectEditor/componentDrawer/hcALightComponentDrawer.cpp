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

    bool enabled = light.isEnabled();
    if (ImGui::Checkbox("Enabled", &enabled))
      light.setEnabled(enabled);

    bool shadowsEnabled = light.isShadowsEnabled();
    if (ImGui::Checkbox("Shadows Enabled", &shadowsEnabled))
      light.setShadowsEnabled(shadowsEnabled);

    Color color = light.getColor();
    if (imguiUtilities::DrawColorEdit3("Color", color))
      light.setColor(color);

    float intensity = light.getIntensity();
    if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f))
      light.setIntensity(intensity);

    float range = light.getRange();
    if (ImGui::DragFloat("Range", &range, 0.5f, 0.0f))
      light.setRange(range);
  }

  ALightComponentDrawer::ALightComponentDrawer(componentType::Type type) :
    ABaseComponentDrawer<ALightComponent>(type)
  {}
}
