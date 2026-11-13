#include "hc/scene/lights/hcLightManager.h"

#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/lights/hcLight.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/scene/gameObject/components/hcIComponent.h"
#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/graphics/lightShadowManager/hcILightShadowMapManager.h"
#include "hc/graphics/resource/dataBlock/hcIDataBlockManager.h"

namespace hc
{
  LightManager::LightManager() :
    m_lightData(),
    m_lightShadowData(),
    m_directionalLights(),
    m_omniLights(),
    m_spotLights(),
    m_components(),
    m_generatedDirectionalLightShadows(),
    m_generatedSpotLightShadows()
  {}

  LightManager::~LightManager()
  {}

  void LightManager::initialize()
  {
    // TODO: Initialize light manager resources if needed
  }

  void LightManager::prepareLightDataForRendering(
    const SceneGraph& sceneGraph,
    graphics::shadows::ILightShadowMapManager& lightShadowManager,
    IDataBlockManager& dataBlockManager
  )
  {
    lightShadowManager.clear();
    m_directionalLights.clear();
    m_omniLights.clear();
    m_spotLights.clear();
    m_generatedDirectionalLightShadows.clear();
    m_generatedSpotLightShadows.clear();

    // TODO
    //
    // Lights should be filtered based on the camera's view frustum to only include lights
    // that affect the visible scene. For now, we gather all lights in the scene graph
    // without filtering.

    gatherLightWorkItems(
      sceneGraph,
      m_directionalLights,
      m_omniLights,
      m_spotLights
    );

    generateLightShadowData(
      sceneGraph,
      lightShadowManager,
      m_directionalLights,
      m_spotLights,
      m_generatedDirectionalLightShadows,
      m_generatedSpotLightShadows
    );

    uploadLightDataToGPU(
      m_directionalLights,
      m_omniLights,
      m_spotLights,
      dataBlockManager
    );

    uploadLightShadowDataToGPU(
      m_generatedDirectionalLightShadows,
      m_generatedSpotLightShadows,
      dataBlockManager
    );

    // Bind blocks data
    dataBlockManager.bind(dataBlockType::Lights);
    dataBlockManager.bind(dataBlockType::LightShadows);

    m_directionalLights.clear();
    m_omniLights.clear();
    m_spotLights.clear();
    m_generatedDirectionalLightShadows.clear();
    m_generatedSpotLightShadows.clear();
  }

  void LightManager::clear()
  {
    // clear light manager resources if needed
  }

  void LightManager::destroy()
  {
    // destroy light manager if needed
  }

  void LightManager::gatherLightWorkItems(
    const SceneGraph& sceneGraph,
    Vector<DirectionalLightWorkItem>& outDirectionalLights,
    Vector<OmniLightWorkItem>& outOmniLights,
    Vector<SpotLightWorkItem>& outSpotLights
  )
  {
    GameObject* root = sceneGraph.getRoot();
    if (root != nullptr)
    {
      gatherLightWorkItemsFromGameObject(
        root,
        outDirectionalLights,
        outOmniLights,
        outSpotLights
      );
    }
  }

  void LightManager::gatherLightWorkItemsFromGameObject(
    GameObject* gameObject,
    Vector<DirectionalLightWorkItem>& outDirectionalLights,
    Vector<OmniLightWorkItem>& outOmniLights,
    Vector<SpotLightWorkItem>& outSpotLights
  )
  {
    m_components.clear();
    gameObject->getComponents(m_components);

    for (IComponent* component : m_components)
    {
      componentType::Type componentType = component->getType();
      if (componentType != componentType::Type::Light)
        continue;

      LightComponent* lightComponent = reinterpret_cast<LightComponent*>(component);
      scene::lights::Light& light = lightComponent->getLight();
      if (!light.enabled)
        continue;

      scene::lights::lightType::Type lightType = lightComponent->getLightType();
      if (lightType == scene::lights::lightType::Type::Directional)
      {
        if (hasReachedMaxDirectionalLightData(outDirectionalLights.size()))
          continue;

        scene::lights::DirectionalLight* directionalLight = light.getIfDirectionalLight();
        DirectionalLightWorkItem pair;
        pair.light = &light;
        pair.directionalLight = directionalLight;
        pair.lightData = light.getDirectionalLightDataBlockStructure();
        outDirectionalLights.push_back(pair);
      }
      else if (lightType == scene::lights::lightType::Type::Omni)
      {
        if (hasReachedMaxOmniLightData(outOmniLights.size()))
          continue;

        scene::lights::OmniLight* omniLight = light.getIfOmniLight();
        OmniLightWorkItem pair;
        pair.light = &light;
        pair.omniLight = omniLight;
        pair.lightData = light.getOmniLightDataBlockStructure();
        outOmniLights.push_back(pair);
      }
      else if (lightType == scene::lights::lightType::Type::Spot)
      {
        if (hasReachedMaxSpotLightData(outSpotLights.size()))
          continue;

        scene::lights::SpotLight* spotLight = light.getIfSpotLight();
        SpotLightWorkItem pair;
        pair.light = &light;
        pair.spotLight = spotLight;
        pair.lightData = light.getSpotLightDataBlockStructure();
        outSpotLights.push_back(pair);
      }
    }

    Vector<GameObject*> children;
    gameObject->getChildren(children);
    for (GameObject* child : children)
    {
      gatherLightWorkItemsFromGameObject(
        child,
        outDirectionalLights,
        outOmniLights,
        outSpotLights
      );
    }
  }

  void LightManager::generateLightShadowData(
    const SceneGraph& sceneGraph,
    graphics::shadows::ILightShadowMapManager& lightShadowMapManager,
    Vector<DirectionalLightWorkItem>& directionalLights,
    Vector<SpotLightWorkItem>& spotLights,
    Vector<dataBlockStructure::DirectionalLightShadow>& outDirectionalLightShadows,
    Vector<dataBlockStructure::SpotLightShadow>& outSpotLightShadows
  )
  {
    for (DirectionalLightWorkItem& directionalLightItem : directionalLights)
    {
      if (!directionalLightItem.light->shadowsEnabled
        || hasReachedMaxDirectionalLightShadows(outDirectionalLightShadows.size())
        || lightShadowMapManager.hasReachedMaxDirectionalLightShadows())
      {
        directionalLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      dataBlockStructure::DirectionalLightShadow shadowData = lightShadowMapManager
        .generateDirectionalLightShadowTexture(
          *directionalLightItem.light,
          sceneGraph
        );

      if (shadowData.shadowMapIndex < 0)
      {
        directionalLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      directionalLightItem.lightData.shadowFrameDataIndex = static_cast<Int32>(outDirectionalLightShadows.size());
      outDirectionalLightShadows.push_back(shadowData);
    }

    for (SpotLightWorkItem& spotLightItem : spotLights)
    {
      if (!spotLightItem.light->shadowsEnabled
        || hasReachedMaxSpotLightShadows(outSpotLightShadows.size())
        || lightShadowMapManager.hasReachedMaxSpotLightShadows())
      {
        spotLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      dataBlockStructure::SpotLightShadow shadowData = lightShadowMapManager.generateSpotLightShadowTexture(
        *spotLightItem.light,
        sceneGraph
      );

      if (shadowData.shadowMapIndex < 0)
      {
        spotLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      spotLightItem.lightData.shadowFrameDataIndex = static_cast<Int32>(outSpotLightShadows.size());
      outSpotLightShadows.push_back(shadowData);
    }

    // TODO Omni lights
  }

  void LightManager::uploadLightDataToGPU(
    const Vector<DirectionalLightWorkItem>& directionalLights,
    const Vector<OmniLightWorkItem>& omniLights,
    const Vector<SpotLightWorkItem>& spotLights,
    IDataBlockManager& blockDataManager)
  {
    m_lightData.numDirectionalLights = static_cast<Int32>(directionalLights.size());
    for (SizeT i = 0; i < directionalLights.size(); ++i)
      m_lightData.directionalLights[i] = directionalLights[i].lightData;

    m_lightData.numOmniLights = static_cast<Int32>(omniLights.size());
    for (SizeT i = 0; i < omniLights.size(); ++i)
      m_lightData.omniLights[i] = omniLights[i].lightData;

    m_lightData.numSpotLights = static_cast<Int32>(spotLights.size());
    for (SizeT i = 0; i < spotLights.size(); ++i)
      m_lightData.spotLights[i] = spotLights[i].lightData;

    blockDataManager.upload(dataBlockType::Lights, &m_lightData);
    blockDataManager.bind(dataBlockType::Lights);
  }

  void LightManager::uploadLightShadowDataToGPU(
    const Vector<dataBlockStructure::DirectionalLightShadow>& directionalLightShadows,
    const Vector<dataBlockStructure::SpotLightShadow>& spotLightShadows,
    IDataBlockManager& blockDataManager
  )
  {
    for (SizeT i = 0; i < directionalLightShadows.size(); ++i)
      m_lightShadowData.directionalLightShadowData[i] = directionalLightShadows[i];

    for (SizeT i = 0; i < spotLightShadows.size(); ++i)
      m_lightShadowData.spotLightShadowData[i] = spotLightShadows[i];

    blockDataManager.upload(dataBlockType::LightShadows, &m_lightShadowData);
    blockDataManager.bind(dataBlockType::LightShadows);
  }

  bool LightManager::hasReachedMaxDirectionalLightData(SizeT numDirectionalLights) const
  {
    return numDirectionalLights >= dataBlockStructure::Lights::MAX_DIRECTIONAL_LIGHTS;
  }

  bool LightManager::hasReachedMaxOmniLightData(SizeT numOmniLights) const
  {
    return numOmniLights >= dataBlockStructure::Lights::MAX_OMNI_LIGHTS;
  }

  bool LightManager::hasReachedMaxSpotLightData(SizeT numSpotLights) const
  {
    return numSpotLights >= dataBlockStructure::Lights::MAX_SPOT_LIGHTS;
  }

  bool LightManager::hasReachedMaxDirectionalLightShadows(SizeT numGeneratedDirectionalLightShadows) const
  {
    return numGeneratedDirectionalLightShadows >= dataBlockStructure::LightShadows::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA;
  }

  bool LightManager::hasReachedMaxSpotLightShadows(SizeT numGeneratedSpotLightShadows) const
  {
    return  numGeneratedSpotLightShadows >= dataBlockStructure::LightShadows::MAX_SPOT_LIGHTS_SHADOW_DATA;
  }
}
