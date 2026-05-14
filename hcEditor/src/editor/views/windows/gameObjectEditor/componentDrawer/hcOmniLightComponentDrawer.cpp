#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcOmniLightComponentDrawer.h"

namespace hc::editor
{
  OmniLightComponentDrawer::OmniLightComponentDrawer() :
    ALightComponentDrawer(componentType::OmniLight)
  {}

  void OmniLightComponentDrawer::onDrawComponent(ALightComponent * component)
  {
    ALightComponentDrawer::onDrawComponent(component);

    OmniLightComponent* omniLightComponent =
      reinterpret_cast<OmniLightComponent*>(component);

    OmniLight& omniLight = omniLightComponent->getOmniLight();

    float range = omniLight.getRange();
    if (ImGui::InputFloat(
      "Range",
      &range,
      0.1f,
      1.0f,
      "%.3f"
    ))
    {
      omniLight.setRange(range);
    }
  }
}
