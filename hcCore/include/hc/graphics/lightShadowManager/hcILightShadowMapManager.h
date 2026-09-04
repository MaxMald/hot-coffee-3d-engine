#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneGraph;

  /**
   * @brief Interface for managing light shadow textures and their generation in the
   * rendering engine.
   */
  class HC_CORE_EXPORT ILightShadowMapManager
  {
  public:
    static constexpr SizeT DIRECTIONAL_SHADOW_MAP_TEXTURE_SIZE = 2048;

    virtual ~ILightShadowMapManager();

    /**
     * @brief Binds the shadow textures for reading in the shader programs. This method
     * should be called before rendering to ensure that the correct shadow textures are
     * available for sampling.
     *
     * Textures are binded to specific texture units, see
     * SAMPLER_2D_BINDING_POINT_DIRECTIONAL_SHADOW_MAP and
     * SAMPLER_2D_BINDING_POINT_SPOT_LIGHT_SHADOW_MAP in hcGraphicsCommons.h for details.
     */
    virtual void bindShadowTexturesForReading() = 0;

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
     * @brief Generates a new spot light shadow texture and returns its index. If the
     * maximum number of spot light shadows has been reached, this method returns -1.
     *
     * @param lightPosition The position of the spot light in world space.
     * @param lightViewProjectionMatrix The view-projection matrix of the spot light.
     * @param sceneGraph The scene graph containing the objects that may cast shadows for
     * this light.
     *
     * @return The index of the newly generated spot light shadow texture, or -1 if the
     * maximum number of shadows has been reached.
     */
    virtual Int32 generateSpotLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) = 0;

    /**
     * @brief Clears the light shadow map manager.
     */
    virtual void clear() = 0;

    /**
     * @brief Checks if the maximum number of directional light shadows has been reached.
     *
     * @return True if the maximum number of directional light shadows has been reached,
     * false otherwise.
     */
    virtual bool hasReachedMaxDirectionalLightShadows() const = 0;
    
    /**
     * @brief Checks if the maximum number of spot light shadows has been reached.
     *
     * @return True if the maximum number of spot light shadows has been reached, false
     * otherwise.
     */
    virtual bool hasReachedMaxSpotLightShadows() const = 0;

    /**
     * @brief Destroys the light shadow map manager and releases any allocated resources.
     */
    virtual void destroy() = 0;

  protected:
    ILightShadowMapManager();
  };
}
