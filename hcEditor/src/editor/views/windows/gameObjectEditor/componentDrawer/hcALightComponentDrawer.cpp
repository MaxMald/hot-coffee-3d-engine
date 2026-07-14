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

    Color color = light.getColor();
    if (imguiUtilities::DrawColorEdit3("Color", color))
      light.setColor(color);

    float intensity = light.getIntensity();
    if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f))
      light.setIntensity(intensity);

    float range = light.getRange();
    if (ImGui::DragFloat("Range", &range, 0.5f, 0.0f))
      light.setRange(range);

    bool shadowsEnabled = light.isShadowsEnabled();
    if (ImGui::Checkbox("Shadows Enabled", &shadowsEnabled))
      light.setShadowsEnabled(shadowsEnabled);

    if (shadowsEnabled)
    {
      float shadowBias = light.getShadowBias();
      if (ImGui::DragFloat("Shadow Bias", &shadowBias, 0.0001f, 0.0f, 1.0f))
        light.setShadowBias(shadowBias);

      float shadowStrength = light.getShadowStrength();
      if (ImGui::DragFloat("Shadow Strength", &shadowStrength, 0.01f, 0.0f, 1.0f))
        light.setShadowStrength(shadowStrength);
    }
  }

  ALightComponentDrawer::ALightComponentDrawer(componentType::Type type) :
    ABaseComponentDrawer<ALightComponent>(type)
  {}
}
