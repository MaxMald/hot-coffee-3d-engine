#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcSpotLightComponentDrawer.h"

namespace hc::editor
{
  SpotLightComponentDrawer::SpotLightComponentDrawer() :
    ALightComponentDrawer(componentType::SpotLight)
  {}

  void SpotLightComponentDrawer::onDrawComponent(ALightComponent * component)
  {
    ALightComponentDrawer::onDrawComponent(component);

    ImGui::Text("Spot Light Settings");
    ImGui::Separator();

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

    float innerConeAngle = spotLight.getInnerConeAngle().toDegrees();
    if (ImGui::DragFloat("Inner Cone Angle", &innerConeAngle, 0.5f))
      spotLight.setInnerConeAngle(Angle::FromDegrees(innerConeAngle));

    float outerConeAngle = spotLight.getOuterConeAngle().toDegrees();
    if (ImGui::DragFloat("Outer Cone Angle", &outerConeAngle, 0.5f))
      spotLight.setOuterConeAngle(Angle::FromDegrees(outerConeAngle));

    ImGui::Text("Spot Light Shadow Settings");
    ImGui::Separator();

    float projectionNearPlane = spotLight.getShadowProjectionNearPlane();
    if (ImGui::InputFloat("Projection Near Plane", &projectionNearPlane, 1.0f, 0.1f, "%.3f"))
      spotLight.setShadowProjectionNearPlane(projectionNearPlane);

    float projectionFarPlane = spotLight.getShadowProjectionFarPlane();
    if (ImGui::InputFloat("Projection Far Plane", &projectionFarPlane, 1.0f, 0.1f, "%.3f"))
      spotLight.setShadowProjectionFarPlane(projectionFarPlane);
  }
}
