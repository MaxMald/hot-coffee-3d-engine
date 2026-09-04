#include "hc/scene/light/hcLightManager.h"
#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/light/hcALight.h"
#include "hc/scene/light/hcDirectionalLight.h"
#include "hc/scene/light/hcOmniLight.h"
#include "hc/scene/light/hcSpotLight.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/scene/gameObject/components/hcIComponent.h"
#include "hc/scene/gameObject/components/hcALightComponent.h"
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
    ILightShadowMapManager& lightShadowManager,
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
      m_generatedSpotLightShadows,
      dataBlockManager.shouldTransposeMatrices()
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

  void LightManager::destroy()
  {
    // TODO: destroy
  }

  void LightManager::gatherLightWorkItems(
    const SceneGraph& sceneGraph,
    Vector<DirectionalLightWorkItem>& outDirectionalLights,
    Vector<OmniLightWorkItem>& outOmniLights,
    Vector<SpotLightWorkItem>& outSpotLights
  )
  {
    const Vector<UniquePtr<GameObject>>& roots = sceneGraph.getRoots();
    for (const UniquePtr<GameObject>& root : roots)
    {
      gatherLightWorkItemsFromGameObject(
        root.get(),
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
      if (componentType == componentType::DirectionalLight)
      {
        if (hasReachedMaxDirectionalLightData(outDirectionalLights.size()))
          continue;

        ALightComponent* lightComponent = reinterpret_cast<ALightComponent*>(component);
        ALight& light = lightComponent->getLight();

        if (light.isEnabled())
        {
          DirectionalLight& directionalLight = static_cast<DirectionalLight&>(light);
          DirectionalLightWorkItem pair;
          pair.light = &directionalLight;
          pair.lightData = directionalLight.getDataBlockStructure();
          outDirectionalLights.push_back(pair);
        }

        continue;
      }
      else if (componentType == componentType::OmniLight)
      {
        if (hasReachedMaxOmniLightData(outOmniLights.size()))
          continue;

        ALightComponent* lightComponent = reinterpret_cast<ALightComponent*>(component);
        ALight& light = lightComponent->getLight();

        if (light.isEnabled())
        {
          OmniLight& omniLight = static_cast<OmniLight&>(light);
          OmniLightWorkItem pair;
          pair.light = &omniLight;
          pair.lightData = omniLight.getDataBlockStructure();
          outOmniLights.push_back(pair);
        }

        continue;
      }
      else if (componentType == componentType::SpotLight)
      {
        if (hasReachedMaxSpotLightData(outSpotLights.size()))
          continue;

        ALightComponent* lightComponent = reinterpret_cast<ALightComponent*>(component);
        ALight& light = lightComponent->getLight();

        if (light.isEnabled())
        {
          SpotLight& spotLight = static_cast<SpotLight&>(light);
          SpotLightWorkItem pair;
          pair.light = &spotLight;
          pair.lightData = spotLight.getDataBlockStructure();
          outSpotLights.push_back(pair);
        }

        continue;
      }
    }

    const Vector<UniquePtr<GameObject>>& children = gameObject->getChildren();
    for (const UniquePtr<GameObject>& child : children)
    {
      gatherLightWorkItemsFromGameObject(
        child.get(),
        outDirectionalLights,
        outOmniLights,
        outSpotLights
      );
    }
  }

  void LightManager::generateLightShadowData(
    const SceneGraph& sceneGraph,
    ILightShadowMapManager& lightShadowMapManager,
    Vector<DirectionalLightWorkItem>& directionalLights,
    Vector<SpotLightWorkItem>& spotLights,
    Vector<dataBlockStructure::DirectionalLightShadow>& outDirectionalLightShadows,
    Vector<dataBlockStructure::SpotLightShadow>& outSpotLightShadows,
    bool transposeMatrices
  )
  {
    for (DirectionalLightWorkItem& directionalLightItem : directionalLights)
    {
      if (!directionalLightItem.light->isShadowsEnabled()
        || hasReachedMaxDirectionalLightShadows(outDirectionalLightShadows.size())
        || lightShadowMapManager.hasReachedMaxDirectionalLightShadows())
      {
        directionalLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      dataBlockStructure::DirectionalLightShadow lightShadowData
        = directionalLightItem.light->getShadowDataBlockStructure(transposeMatrices);

      Int32 shadowMapIndex = lightShadowMapManager.generateDirectionalLightShadowTexture(
        directionalLightItem.light->getPosition(),
        lightShadowData.LightViewProjectionMatrix,
        sceneGraph
      );

      if (shadowMapIndex < 0)
      {
        directionalLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      lightShadowData.shadowMapIndex = shadowMapIndex;
      directionalLightItem.lightData.shadowFrameDataIndex = outDirectionalLightShadows.size();
      outDirectionalLightShadows.push_back(lightShadowData);
    }

    for (SpotLightWorkItem& spotLightItem : spotLights)
    {
      if (!spotLightItem.light->isShadowsEnabled()
        || hasReachedMaxSpotLightShadows(outSpotLightShadows.size())
        || lightShadowMapManager.hasReachedMaxSpotLightShadows())
      {
        spotLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      dataBlockStructure::SpotLightShadow lightShadowData
        = spotLightItem.light->getShadowDataBlockStructure(transposeMatrices);

      Int32 shadowMapIndex = lightShadowMapManager.generateSpotLightShadowTexture(
        spotLightItem.light->getPosition(),
        lightShadowData.lightViewProjectionMatrix,
        sceneGraph
      );

      if (shadowMapIndex < 0)
      {
        spotLightItem.lightData.shadowFrameDataIndex = -1;
        continue;
      }

      lightShadowData.shadowMapIndex = shadowMapIndex;
      spotLightItem.lightData.shadowFrameDataIndex = outSpotLightShadows.size();
      outSpotLightShadows.push_back(lightShadowData);
    }
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
