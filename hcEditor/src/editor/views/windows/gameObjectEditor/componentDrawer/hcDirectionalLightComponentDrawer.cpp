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
  }
}
