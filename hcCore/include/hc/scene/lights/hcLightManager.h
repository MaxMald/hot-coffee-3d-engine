#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  namespace scene::lights
  {
    class Light;
    struct DirectionalLight;
    struct OmniLight;
    struct SpotLight;
  }

  class SceneGraph;
  class GameObject;
  class IComponent;
  class ILightShadowMapManager;
  class IDataBlockManager;

  struct DirectionalLightWorkItem
  {
    dataBlockStructure::DirectionalLight lightData;     ///< The data block structure representing the directional light's properties for GPU usage.
    scene::lights::DirectionalLight* directionalLight;  ///< Pointer to the actual DirectionalLight object in the scene.
    scene::lights::Light* light;                        ///< Pointer to the base Light object.
  };

  struct OmniLightWorkItem
  {
    dataBlockStructure::OmniLight lightData;  ///< The data block structure representing the omni light's properties for GPU usage.
    scene::lights::OmniLight* omniLight;      ///< Pointer to the actual OmniLight object in the scene.
    scene::lights::Light* light;              ///< Pointer to the base Light object.
  };

  struct SpotLightWorkItem
  {
    dataBlockStructure::SpotLight lightData;  ///< The data block structure representing the spot light's properties for GPU usage.
    scene::lights::SpotLight* spotLight;      ///< Pointer to the actual SpotLight object in the scene.
    scene::lights::Light* light;              ///< Pointer to the base Light object.
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
    void clear();
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
      Vector<dataBlockStructure::SpotLightShadow>& outSpotLightShadows
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
