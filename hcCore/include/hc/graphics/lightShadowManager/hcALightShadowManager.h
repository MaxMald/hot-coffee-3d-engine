#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/lightShadowFrameData/hcLightShadowFrameData.h"

namespace hc
{
  class DirectionalLight;
  class GameObject;
  class SceneGraph;

  /**
   * @brief Abstract base class for managing light shadow data.
   *
   * Concrete classes should implement the following methods:
   * - `uploadShadowDataToGPU()`: Uploads the light shadow frame data to the GPU.
   * - `destroy()`: Destroys the light shadow manager and releases any allocated
   *   resources.
   * - `generateDirectionalLightShadowTexture()`: Generates a new directional light shadow
   *   texture and returns its index.
   */
  class HC_CORE_EXPORT ALightShadowManager
  {
  public:
    static constexpr SizeT DIRECTIONAL_SHADOW_MAP_TEXTURE_SIZE = 2048;

    /**
     * @brief Virtual destructor for the ALightShadowManager class. Cleans up any
     * resources used by the manager.
     */
    virtual ~ALightShadowManager();

    /**
     * @brief Updates the light shadow frame data to the GPU. This method should be called
     * once per frame after all shadow data has been generated and before rendering.
     */
    virtual void uploadShadowDataToGPU() = 0;

    /**
     * @brief Destroys the light shadow manager and releases any allocated resources.
     */
    virtual void destroy() = 0;

    /**
     * @brief Clears the light shadow manager.
     */
    void clear();

    /**
     * @brief Generates data for a new directional light shadow and saves it in the light
     * shadow frame data. Returns the index of the newly generated shadow data, or -1 if
     * the maximum number of shadows has been reached.
     *
     * @param directionalLight The directional light for which to generate shadow data.
     * @param sceneGraph The scene graph containing the objects that may cast shadows for
     * this light.
     *
     * @return The index of the newly generated directional light shadow data, or -1 if
     * the maximum number of shadows has been reached.
     */
    Int32 generateDirectionalLightShadowData(
      const DirectionalLight& directionalLight,
      const SceneGraph& sceneGraph
    );

  protected:
    LightShadowFrameData m_lightShadowFrameData;
    SizeT m_countDirectionalLightShadows;

    /**
     * @brief Protected constructor for the ALightShadowManager class.
     */
    ALightShadowManager();

    /**
     * @brief Generates a new directional light shadow texture and returns its index. If
     * the maximum number of directional light shadows has been reached, this method
     * returns -1.
     *
     * @param lightPosition The position of the directional light in world space.
     * @param lightViewProjectionMatrix The view-projection matrix of the directional
     * light.
     * @param sceneGraph The scene graph containing the objects that may cast shadows for
     * this light.
     *
     * @return The index of the newly generated directional light shadow texture, or -1 if
     * the maximum number of shadows has been reached.
     */
    virtual Int32 generateDirectionalLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) = 0;

    /**
     * @brief Checks if the maximum number of directional light shadows has been reached.
     *
     * @return True if the maximum number of directional light shadows has been reached,
     * false otherwise.
     */
    inline bool hasReachedMaxDirectionalLightShadows() const
    {
      return m_countDirectionalLightShadows >= LightShadowFrameData::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA;
    }
  };
}
