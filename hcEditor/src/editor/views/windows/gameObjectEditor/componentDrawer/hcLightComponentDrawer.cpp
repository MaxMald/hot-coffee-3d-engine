#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcLightComponentDrawer.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  static const char* typeNames[] = {
    "Directional",
    "Point",
    "Spot"
  };

  LightComponentDrawer::LightComponentDrawer() :
    ABaseComponentDrawer<LightComponent>(componentType::Light)
  {
  }

  LightComponentDrawer::~LightComponentDrawer()
  {
  }

  void LightComponentDrawer::onDrawComponent(LightComponent* component)
  {
    if (!component)
      return;

    Light* light = component->getLight();
    if (!light)
    {
      ImGui::Text("No light created");
      if (ImGui::Button("Create Light"))
        component->create();

      return;
    }

    Int32 currentTypeIndex = getLightTypeIndex(light);
    if (currentTypeIndex == -1)
    {
      ImGui::Text("Invalid light type");
      return;
    }

    if (ImGui::Combo("Light Type", &currentTypeIndex, typeNames, IM_ARRAYSIZE(typeNames)))
    {
      try
      {
        lightType::Type newType = lightType::fromString(typeNames[currentTypeIndex]);
        light->setType(newType);
      }
      catch (const Exception& e)
      {
        LogService::Error("Failed to set light type: " + String(e.what()));
        return;
      }
    }

    // ADDITIONAL PROPERTIES

    Color color = light->getColor();
    if (imguiUtilities::DrawColorEdit3("Color", color))
      light->setColor(color);

    float intensity = light->getIntensity();
    if (ImGui::InputFloat("Intensity", &intensity))
      light->setIntensity(intensity);

    bool enabled = light->isEnabled();
    if (ImGui::Checkbox("Enabled", &enabled))
      light->setEnabled(enabled);
  }

  Int32 LightComponentDrawer::getLightTypeIndex(Light* light) const
  {
    if (!light)
      return -1;

    switch (light->getType())
    {
    case lightType::Type::Directional:
      return 0;
    case lightType::Type::Point:
      return 1;
    case lightType::Type::Spot:
      return 2;
    default:
      return -1;
    }
  }
}
