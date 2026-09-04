#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  class SceneGraph;
  class GameObject;
  class IComponent;
  class DirectionalLight;
  class OmniLight;
  class SpotLight;
  class ILightShadowMapManager;
  class IDataBlockManager;

  struct alignas(16) DirectionalLightWorkItem
  {
    dataBlockStructure::DirectionalLight lightData;
    DirectionalLight* light;
  };

  struct alignas(16) OmniLightWorkItem
  {
    dataBlockStructure::OmniLight lightData;
    OmniLight* light;
  };

  struct alignas(16) SpotLightWorkItem
  {
    dataBlockStructure::SpotLight lightData;
    SpotLight* light;
  };

  class HC_CORE_EXPORT LightManager
  {
  public:
    LightManager();
    ~LightManager();

    void initialize();
    void prepareLightDataForRendering(
      const SceneGraph& sceneGraph,
      ILightShadowMapManager& lightShadowMapManager,
      IDataBlockManager& blockDataManager
    );
    void destroy();

  private:
    dataBlockStructure::Lights m_lightData;
    dataBlockStructure::LightShadows m_lightShadowData;
    Vector<DirectionalLightWorkItem> m_directionalLights;
    Vector<OmniLightWorkItem> m_omniLights;
    Vector<SpotLightWorkItem> m_spotLights;
    Vector<dataBlockStructure::DirectionalLightShadow> m_generatedDirectionalLightShadows;
    Vector<dataBlockStructure::SpotLightShadow> m_generatedSpotLightShadows;
    Vector<IComponent*> m_components;

    void gatherLightWorkItems(
      const SceneGraph& sceneGraph,
      Vector<DirectionalLightWorkItem>& outDirectionalLights,
      Vector<OmniLightWorkItem>& outOmniLights,
      Vector<SpotLightWorkItem>& outSpotLights
    );

    void gatherLightWorkItemsFromGameObject(
      GameObject* gameObject,
      Vector<DirectionalLightWorkItem>& outDirectionalLights,
      Vector<OmniLightWorkItem>& outOmniLights,
      Vector<SpotLightWorkItem>& outSpotLights
    );

    void generateLightShadowData(
      const SceneGraph& sceneGraph,
      ILightShadowMapManager& lightShadowManager,
      Vector<DirectionalLightWorkItem>& directionalLights,
      Vector<SpotLightWorkItem>& spotLights,
      Vector<dataBlockStructure::DirectionalLightShadow>& outDirectionalLightShadows,
      Vector<dataBlockStructure::SpotLightShadow>& outSpotLightShadows,
      bool transposeMatrices
    );

    void uploadLightDataToGPU(
      const Vector<DirectionalLightWorkItem>& outDirectionalLights,
      const Vector<OmniLightWorkItem>& outOmniLights,
      const Vector<SpotLightWorkItem>& outSpotLights,
      IDataBlockManager& blockDataManager
    );

    void uploadLightShadowDataToGPU(
      const Vector<dataBlockStructure::DirectionalLightShadow>& outDirectionalLightShadows,
      const Vector<dataBlockStructure::SpotLightShadow>& outSpotLightShadows,
      IDataBlockManager& blockDataManager
    );

    bool hasReachedMaxDirectionalLightData(SizeT numDirectionalLights) const;
    bool hasReachedMaxOmniLightData(SizeT numOmniLights) const;
    bool hasReachedMaxSpotLightData(SizeT numSpotLights) const;
    bool hasReachedMaxDirectionalLightShadows(SizeT numGeneratedDirectionalLightShadows) const;
    bool hasReachedMaxSpotLightShadows(SizeT numGeneratedSpotLightShadows) const;
  };
}
