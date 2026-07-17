#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcDirectionalLightComponentDrawer.h"

namespace hc::editor
{
  DirectionalLightComponentDrawer::DirectionalLightComponentDrawer() :
    ALightComponentDrawer(componentType::DirectionalLight)
  {}

  void DirectionalLightComponentDrawer::onDrawComponent(ALightComponent * component)
  {
    ALightComponentDrawer::onDrawComponent(component);

    ImGui::Text("Directional Light Settings");
    ImGui::Separator();

    DirectionalLightComponent* directionalLightComponent =
      reinterpret_cast<DirectionalLightComponent*>(component);

    DirectionalLight& directionalLight = directionalLightComponent->getDirectionalLight();
    Vector3f direction = directionalLight.getDirection();
    ImGui::InputFloat3(
      "Direction",
      &(direction.x),
      "%.3f",
      ImGuiInputTextFlags_ReadOnly
    );

    ImGui::Text("Directional Light Shadow Settings");
    ImGui::Separator();

    Vector3f shadowViewTarget = directionalLight.getShadowViewTarget();
    if (ImGui::InputFloat3("Shadow View Target", &(shadowViewTarget.x), "%.3f"))
      directionalLight.setShadowViewTarget(shadowViewTarget);

    float shadowViewDistance = directionalLight.getShadowViewDistance();
    if (ImGui::InputFloat("Shadow View Distance", &shadowViewDistance, 0.1f, 1.0f, "%.3f"))
      directionalLight.setShadowViewDistance(shadowViewDistance);

    float projectionSize = directionalLight.getShadowProjectionSize();
    if (ImGui::InputFloat("Projection Size", &projectionSize, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionSize(projectionSize);

    float nearPlane = directionalLight.getShadowProjectionNearPlane();
    if (ImGui::InputFloat("Projection Near Plane", &nearPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionNearPlane(nearPlane);

    float farPlane = directionalLight.getShadowProjectionFarPlane();
    if (ImGui::InputFloat("Projection Far Plane", &farPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight.setShadowProjectionFarPlane(farPlane);
  }
}
