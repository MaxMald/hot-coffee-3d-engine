#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcSpotLightComponentDrawer.h"

namespace hc::editor
{
  SpotLightComponentDrawer::SpotLightComponentDrawer() :
    ALightComponentDrawer(componentType::SpotLight)
  {}

  void SpotLightComponentDrawer::onDrawComponent(ALightComponent * component)
  {
    ALightComponentDrawer::onDrawComponent(component);

    SpotLightComponent* spotLightComponent =
      reinterpret_cast<SpotLightComponent*>(component);

    SpotLight& spotLight = spotLightComponent->getSpotLight();
    Vector3 direction = spotLight.getDirection();

    ImGui::InputFloat3(
      "Direction",
      &(direction.x),
      "%.3f",
      ImGuiInputTextFlags_ReadOnly
    );

    float innerConeAngle = spotLight.getInnerConeAngle();
    if (ImGui::DragFloat("Inner Cone Angle", &innerConeAngle, 0.5f))
      spotLight.setInnerConeAngle(innerConeAngle);

    float outerConeAngle = spotLight.getOuterConeAngle();
    if (ImGui::DragFloat("Outer Cone Angle", &outerConeAngle, 0.5f))
      spotLight.setOuterConeAngle(outerConeAngle);
  }
}
