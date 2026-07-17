#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcDirectionalLightComponentDrawer.h"

namespace hc::editor
{
  DirectionalLightComponentDrawer::DirectionalLightComponentDrawer() :
    ALightComponentDrawer(componentType::DirectionalLight)
  {}

  void DirectionalLightComponentDrawer::onDrawComponent(ALightComponent * component)
  {
    ALightComponentDrawer::onDrawComponent(component);

    DirectionalLightComponent* directionalLightComponent =
      reinterpret_cast<DirectionalLightComponent*>(component);

    DirectionalLight& directionalLight = directionalLightComponent->getDirectionalLight();
    Vector3 direction = directionalLight.getDirection();
    ImGui::InputFloat3(
      "Direction",
      &(direction.x),
      "%.3f",
      ImGuiInputTextFlags_ReadOnly
    );

    ImGui::Text("Shadow Projection");
    ImGui::Separator();

    float projectionSize = directionalLight.getShadowProjectionSize();
    if (ImGui::InputFloat("Size", &projectionSize, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionSize(projectionSize);

    float nearPlane = directionalLight.getShadowProjectionNearPlane();
    if (ImGui::InputFloat("Near Plane", &nearPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionNearPlane(nearPlane);

    float farPlane = directionalLight.getShadowProjectionFarPlane();
    if (ImGui::InputFloat("Far Plane", &farPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionFarPlane(farPlane);
  }
}
