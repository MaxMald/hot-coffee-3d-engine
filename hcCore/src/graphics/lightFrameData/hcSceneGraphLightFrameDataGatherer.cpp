#include "hc/graphics/lightFrameData/hcSceneGraphLightFrameDataGatherer.h"

#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/components/hcOmniLightComponent.h"
#include "hc/scene/gameObject/components/hcSpotLightComponent.h"
#include "hc/graphics/lightShadowManager/hcALightShadowManager.h"

namespace hc
{
  void SceneGraphLightFrameDataGatherer::Gather(
    const SceneGraph& sceneGraph,
    LightFrameData& lightFrameData,
    ALightShadowManager& lightShadowManager
  )
  {
    // TODO
    //
    // Lights should be filtered based on the camera's view frustum to only include lights
    // that affect the visible scene. For now, we gather all lights in the scene graph
    // without filtering.

    if (lightFrameData.numDirectionalLights >= LightFrameData::MAX_DIRECTIONAL_LIGHTS &&
      lightFrameData.numOmniLights >= LightFrameData::MAX_OMNI_LIGHTS &&
      lightFrameData.numSpotLights >= LightFrameData::MAX_SPOT_LIGHTS)
    {
      LogService::Warning(
        "Maximum number of lights reached. Additional lights in the scene will be ignored."
      );

      return;
    }

    const Vector<UniquePtr<GameObject>>& roots = sceneGraph.getRoots();
    for (const UniquePtr<GameObject>& root : roots)
    {
      if (!GatherFromGameObject(root, lightFrameData, sceneGraph, lightShadowManager))
        break;
    }
  }

  bool SceneGraphLightFrameDataGatherer::GatherFromGameObject(
    const UniquePtr<GameObject>& gameObject,
    LightFrameData& lightFrameData,
    const SceneGraph& sceneGraph,
    ALightShadowManager& lightShadowManager
  )
  {
    DirectionalLightComponent* directionalLightComponent =
      gameObject->getComponent<DirectionalLightComponent>();

    if (directionalLightComponent)
    {
      GatherFromDirectionalLightComponent(
        directionalLightComponent,
        lightFrameData,
        sceneGraph,
        lightShadowManager
      );
    }

    OmniLightComponent* omniLightComponent = gameObject->getComponent<OmniLightComponent>();
    if (omniLightComponent)
      GatherFromOmniLightComponent(omniLightComponent, lightFrameData);

    SpotLightComponent* spotLightComponent = gameObject->getComponent<SpotLightComponent>();
    if (spotLightComponent)
      GatherFromSpotLightComponent(spotLightComponent, lightFrameData);

    if (lightFrameData.numDirectionalLights >= LightFrameData::MAX_DIRECTIONAL_LIGHTS &&
      lightFrameData.numOmniLights >= LightFrameData::MAX_OMNI_LIGHTS &&
      lightFrameData.numSpotLights >= LightFrameData::MAX_SPOT_LIGHTS)
    {
      LogService::Warning(
        "Maximum number of lights reached. Additional lights in the scene will be ignored."
      );

      return false;
    }

    for (const UniquePtr<GameObject>& child : gameObject->getChildren())
    {
      if (!GatherFromGameObject(child, lightFrameData, sceneGraph, lightShadowManager))
        return false;
    }

    return true;
  }

  void SceneGraphLightFrameDataGatherer::GatherFromDirectionalLightComponent(
    DirectionalLightComponent* directionalLightComponent,
    LightFrameData& lightFrameData,
    const SceneGraph& sceneGraph,
    ALightShadowManager& lightShadowManager
  )
  {
    if (!directionalLightComponent->getLight().isEnabled())
      return;

    if (lightFrameData.numDirectionalLights >= LightFrameData::MAX_DIRECTIONAL_LIGHTS)
    {
      LogService::Warning(
        String::Format(
          "Maximum number of directional lights (%u) exceeded. Additional directional lights will be ignored.",
          LightFrameData::MAX_DIRECTIONAL_LIGHTS
        )
      );
      return;
    }

    Int32 directionalLightIndex = lightFrameData.numDirectionalLights++;
    lightFrameData.directionalLights[directionalLightIndex] =
      directionalLightComponent->getDirectionalLight().toFrameData();

    if (directionalLightComponent->getDirectionalLight().isShadowsEnabled())
    {
      lightFrameData.directionalLights[directionalLightIndex].shadowFrameDataIndex =
        lightShadowManager.generateDirectionalLightShadowData(
          directionalLightComponent->getDirectionalLight(),
          sceneGraph
        );
    }
  }

  void SceneGraphLightFrameDataGatherer::GatherFromOmniLightComponent(
    OmniLightComponent* omniLightComponent,
    LightFrameData& lightFrameData
  )
  {
    if (!omniLightComponent->getLight().isEnabled())
      return;

    if (lightFrameData.numOmniLights >= LightFrameData::MAX_OMNI_LIGHTS)
    {
      LogService::Warning(
        String::Format(
          "Maximum number of omni lights (%u) exceeded. Additional omni lights will be ignored.",
          LightFrameData::MAX_OMNI_LIGHTS
        )
      );
      return;
    }

    Int32 omniLightIndex = lightFrameData.numOmniLights++;
    lightFrameData.omniLights[omniLightIndex] =
      omniLightComponent->getOmniLight().toFrameData();
  }

  void SceneGraphLightFrameDataGatherer::GatherFromSpotLightComponent(
    SpotLightComponent* spotLightComponent,
    LightFrameData& lightFrameData
  )
  {
    if (!spotLightComponent->getLight().isEnabled())
      return;

    if (lightFrameData.numSpotLights >= LightFrameData::MAX_SPOT_LIGHTS)
    {
      LogService::Warning(
        String::Format(
          "Maximum number of spot lights (%u) exceeded. Additional spot lights will be ignored.",
          LightFrameData::MAX_SPOT_LIGHTS
        )
      );
      return;
    }

    Int32 spotLightIndex = lightFrameData.numSpotLights++;
    lightFrameData.spotLights[spotLightIndex] =
      spotLightComponent->getSpotLight().toFrameData();
  }
}
