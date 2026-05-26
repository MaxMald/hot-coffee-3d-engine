#include "hc/graphics/lightFrameData/hcSceneGraphLightFrameDataGatherer.h"

#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/components/hcOmniLightComponent.h"
#include "hc/scene/gameObject/components/hcSpotLightComponent.h"

namespace hc
{
  LightFrameData SceneGraphLightFrameDataGatherer::Gather(
    const SceneGraph& sceneGraph,
    LightFrameData& lightFrameData
  )
  {
    LightFrameData lightFrameData;

    const Vector<UniquePtr<GameObject>>& roots = sceneGraph.getRoots();
    for (const UniquePtr<GameObject>& root : roots)
    {
      if (lightFrameData.numDirectionalLights >= LightFrameData::MAX_DIRECTIONAL_LIGHTS &&
        lightFrameData.numOmniLights >= LightFrameData::MAX_OMNI_LIGHTS &&
        lightFrameData.numSpotLights >= LightFrameData::MAX_SPOT_LIGHTS)
      {
        LogService::Warning(
          "Maximum number of lights reached. Additional lights in the scene will be ignored."
        );
        break;
      }

      GatherFromGameObject(root, lightFrameData);
    }

    return lightFrameData;
  }

  void SceneGraphLightFrameDataGatherer::GatherFromGameObject(
    const UniquePtr<GameObject>& gameObject,
    LightFrameData& lightFrameData
  )
  {
    DirectionalLightComponent* directionalLightComponent =
      gameObject->getComponent<DirectionalLightComponent>();

    if (directionalLightComponent)
      GatherFromDirectionalLightComponent(directionalLightComponent, lightFrameData);

    OmniLightComponent* omniLightComponent = gameObject->getComponent<OmniLightComponent>();
    if (omniLightComponent)
      GatherFromOmniLightComponent(omniLightComponent, lightFrameData);

    SpotLightComponent* spotLightComponent = gameObject->getComponent<SpotLightComponent>();
    if (spotLightComponent)
      GatherFromSpotLightComponent(spotLightComponent, lightFrameData);
  }

  void SceneGraphLightFrameDataGatherer::GatherFromDirectionalLightComponent(
    DirectionalLightComponent* directionalLightComponent,
    LightFrameData& lightFrameData
  )
  {
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
  }

  void SceneGraphLightFrameDataGatherer::GatherFromOmniLightComponent(
    OmniLightComponent* omniLightComponent,
    LightFrameData& lightFrameData
  )
  {
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
