#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/lightFrameData/hcLightFrameData.h"

namespace hc
{
  class SceneGraph;
  class GameObject;
  class DirectionalLightComponent;
  class OmniLightComponent;
  class SpotLightComponent;
  class ALightShadowManager;

  /**
   * @brief Gathers light data from a scene graph.
   *
   * The SceneGraphLightFrameDataGatherer traverses the scene graph to collect
   * information about active lights and compiles it into a LightFrameData
   * structure. This data can then be uploaded to the GPU for use in lighting
   * calculations during rendering.
   */
  class HC_CORE_EXPORT SceneGraphLightFrameDataGatherer
  {
  public:

    /**
     * @brief Gathers light data from the provided scene graph and compiles it into a
     * LightFrameData structure.
     *
     * @param sceneGraph The scene graph to gather light data from.
     * @param lightFrameData Reference to a LightFrameData structure to populate with the
     * gathered data.
     * @param lightShadowManager Reference to an ALightShadowManager for managing light
     * shadow data.
     */
    static void Gather(
      const SceneGraph& sceneGraph,
      LightFrameData& lightFrameData,
      ALightShadowManager& lightShadowManager
    );

  private:
    static bool GatherFromGameObject(
      const UniquePtr<GameObject>& gameObject,
      LightFrameData& lightFrameData,
      const SceneGraph& sceneGraph,
      ALightShadowManager& lightShadowManager
    );

    static void GatherFromDirectionalLightComponent(
      DirectionalLightComponent* directionalLightComponent,
      LightFrameData& lightFrameData,
      const SceneGraph& sceneGraph,
      ALightShadowManager& lightShadowManager
    );

    static void GatherFromOmniLightComponent(
      OmniLightComponent* omniLightComponent,
      LightFrameData& lightFrameData
    );

    static void GatherFromSpotLightComponent(
      SpotLightComponent* spotLightComponent,
      LightFrameData& lightFrameData,
      const SceneGraph& sceneGraph,
      ALightShadowManager& lightShadowManager
    );
  };
}
