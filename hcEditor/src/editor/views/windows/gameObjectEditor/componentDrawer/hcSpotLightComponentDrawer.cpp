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
    if (ImGui::InputFloat(
      "Inner Cone Angle",
      &innerConeAngle,
      0.1f,
      1.0f,
      "%.3f"
    ))
    {
      spotLight.setInnerConeAngle(innerConeAngle);
    }

    float outerConeAngle = spotLight.getOuterConeAngle();
    if (ImGui::InputFloat(
      "Outer Cone Angle",
      &outerConeAngle,
      0.1f,
      1.0f,
      "%.3f"
    ))
    {
      spotLight.setOuterConeAngle(outerConeAngle);
    }
  }
}
