#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcLightComponentDrawer.h"

#include "hc/editor/hcEditorCommons.h"
#include "hc/editor/imgui/hcImgui.h"

namespace hc::editor
{
  LightComponentDrawer::LightComponentDrawer() :
    ABaseComponentDrawer<LightComponent>(componentType::Type::Light)
  {
  }

  LightComponentDrawer::~LightComponentDrawer()
  {
  }

  void LightComponentDrawer::onDrawComponent(LightComponent* component)
  {
    if (!component)
      return;

    component->updateLight();
    scene::lights::Light& light = component->getLight();

    bool enabled = light.enabled;
    if (ImGui::Checkbox("Enabled", &enabled))
      light.enabled = enabled;

    Int32 currentLightTypeIndex = static_cast<Int32>(light.getType());
    if (ImGui::Combo("Light Type", &currentLightTypeIndex, comboItems::LIGHT_TYPE, comboItems::LIGHT_TYPE_COUNT))
      light.setType(static_cast<scene::lights::lightType::Type>(currentLightTypeIndex));
    scene::lights::lightType::Type lightType = light.getType();

    Color color = light.color;
    if (hcImGui::DrawColorEdit3("Color", color))
      light.color = color;

    float intensity = light.intensity;
    if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f))
      light.setIntensity(intensity);

    switch (lightType)
    {
    case scene::lights::lightType::Directional:
      drawDirectionalProperties(light);
      break;
    case scene::lights::lightType::Omni:
      drawOmniProperties(light);
      break;
    case scene::lights::lightType::Spot:
      drawSpotProperties(light);
      break;
    default:
      LogService::Error(
        String::Format(
          "LightComponentDrawer: Unknown light type: %d",
          static_cast<Int32>(lightType)
        )
      );
    }

    if (ImGui::CollapsingHeader("Shadow Settings"))
    {
      bool shadowsEnabled = light.shadowsEnabled;
      if (ImGui::Checkbox("Shadows Enabled", &shadowsEnabled))
        light.shadowsEnabled = shadowsEnabled;

      float shadowBias = light.shadowBias;
      if (ImGui::DragFloat("Shadow Bias", &shadowBias, 0.0001f, 0.0f, 1.0f))
        light.setShadowBias(shadowBias);

      float shadowStrength = light.shadowStrength;
      if (ImGui::DragFloat("Shadow Strength", &shadowStrength, 0.01f, 0.0f, 1.0f))
        light.setShadowStrength(shadowStrength);

      switch (lightType)
      {
      case scene::lights::lightType::Directional:
        drawDirectionalShadowProperties(light);
        break;
      case scene::lights::lightType::Omni:
        drawOmniShadowProperties(light);
        break;
      case scene::lights::lightType::Spot:
        drawSpotShadowProperties(light);
        break;
      default:
        LogService::Error(
          String::Format(
            "LightComponentDrawer: Unknown light type for shadow settings: %d",
            static_cast<Int32>(lightType)
          )
        );
      }
    }
  }

  void LightComponentDrawer::drawDirectionalProperties(scene::lights::Light& light)
  {
    scene::lights::DirectionalLight* directionalLight = light.getIfDirectionalLight();
    if (directionalLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawDirectionalProperties called on non-directional light."
      );
      return;
    }

    Vector3f direction = directionalLight->direction;
    ImGui::InputFloat3(
      "Direction",
      &(direction.x),
      "%.3f",
      ImGuiInputTextFlags_ReadOnly
    );
  }

  void LightComponentDrawer::drawSpotProperties(scene::lights::Light& light)
  {
    scene::lights::SpotLight* spotLight = light.getIfSpotLight();
    if (spotLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawSpotProperties called on non-spot light."
      );
      return;
    }

    Vector3f direction = spotLight->direction;
    ImGui::InputFloat3(
      "Direction",
      &(direction.x),
      "%.3f",
      ImGuiInputTextFlags_ReadOnly
    );

    float range = spotLight->range;
    if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, 100.0f))
      spotLight->setRange(range);

    float innerConeAngle = spotLight->innerConeAngle.toDegrees();
    if (ImGui::DragFloat("Inner Cone Angle", &innerConeAngle, 0.5f))
      spotLight->setInnerConeAngle(Angle::FromDegrees(innerConeAngle));

    float outerConeAngle = spotLight->outerConeAngle.toDegrees();
    if (ImGui::DragFloat("Outer Cone Angle", &outerConeAngle, 0.5f))
      spotLight->setOuterConeAngle(Angle::FromDegrees(outerConeAngle));
  }

  void LightComponentDrawer::drawOmniProperties(scene::lights::Light& light)
  {
    scene::lights::OmniLight* omniLight = light.getIfOmniLight();
    if (omniLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawOmniProperties called on non-omni light."
      );
      return;
    }

    float range = omniLight->range;
    if (ImGui::DragFloat("Range", &range, 0.1f, 0.0f, 100.0f))
      omniLight->setRange(range);
  }

  void LightComponentDrawer::drawDirectionalShadowProperties(scene::lights::Light& light)
  {
    scene::lights::DirectionalLight* directionalLight = light.getIfDirectionalLight();
    if (directionalLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawDirectionalProperties called on non-directional light."
      );
      return;
    }

    Vector3f shadowViewTarget = directionalLight->shadowViewTarget;
    if (ImGui::InputFloat3("Shadow View Target", &(shadowViewTarget.x), "%.3f"))
      directionalLight->shadowViewTarget = shadowViewTarget;

    float shadowViewDistance = directionalLight->shadowViewDistance;
    if (ImGui::InputFloat("Shadow View Distance", &shadowViewDistance, 0.1f, 1.0f, "%.3f"))
      directionalLight->setShadowViewDistance(shadowViewDistance);

    float projectionSize = directionalLight->shadowProjectionSize;
    if (ImGui::InputFloat("Projection Size", &projectionSize, 0.1f, 0.1f, "%.3f"))
      directionalLight->setShadowProjectionSize(projectionSize);

    float nearPlane = directionalLight->shadowProjectionNearPlane;
    if (ImGui::InputFloat("Projection Near Plane", &nearPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight->setShadowProjectionNearPlane(nearPlane);

    float farPlane = directionalLight->shadowProjectionFarPlane;
    if (ImGui::InputFloat("Projection Far Plane", &farPlane, 0.1f, 0.1f, "%.3f"))
      directionalLight->setShadowProjectionFarPlane(farPlane);
  }

  void LightComponentDrawer::drawSpotShadowProperties(scene::lights::Light& light)
  {
    scene::lights::SpotLight* spotLight = light.getIfSpotLight();
    if (spotLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawSpotProperties called on non-spot light."
      );
      return;
    }

    float projectionNearPlane = spotLight->shadowProjectionNearPlane;
    if (ImGui::InputFloat("Projection Near Plane", &projectionNearPlane, 1.0f, 0.1f, "%.3f"))
      spotLight->setShadowProjectionNearPlane(projectionNearPlane);

    float projectionFarPlane = spotLight->shadowProjectionFarPlane;
    if (ImGui::InputFloat("Projection Far Plane", &projectionFarPlane, 1.0f, 0.1f, "%.3f"))
      spotLight->setShadowProjectionFarPlane(projectionFarPlane);
  }

  void LightComponentDrawer::drawOmniShadowProperties(scene::lights::Light& light)
  {
    scene::lights::OmniLight* omniLight = light.getIfOmniLight();
    if (omniLight == nullptr)
    {
      LogService::Error(
        "LightComponentDrawer: drawOmniProperties called on non-omni light."
      );
      return;
    }

    // TODO
  }
}
