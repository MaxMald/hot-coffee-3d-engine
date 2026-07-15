#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class GameObject;

  class HC_CORE_EXPORT ILightShadowTexturesManager
  {
  public:
    virtual ~ILightShadowTexturesManager();

    /**
     * @brief Initializes the light shadow textures manager. This method sets up the
     * necessary resources and prepares the manager for use.
     */
    virtual void initialize() = 0;

    /**
     * @brief Clears the light shadow textures. This method resets the light shadow
     * textures to their default state, effectively removing any previously stored shadow
     * textures.
     */
    virtual void clearLightShadowTextures() = 0;

    /**
     * @brief Generates a new directional light shadow texture and returns its index. If
     * the maximum number of directional light shadows has been reached, this method
     * returns -1.
     *
     * @param lightViewProjectionMatrix The view-projection matrix of the directional
     * light.
     * @param shadowCasters A vector of pointers to GameObjects that cast shadows for this
     * directional light.
     *
     * @return The index of the newly generated directional light shadow texture, or -1 if
     * the maximum number of shadows has been reached.
     */
    virtual Int32 generateDirectionalLightShadowTexture(
      Matrix4 lightViewProjectionMatrix,
      const Vector<GameObject*>& shadowCasters
    ) = 0;

    /**
     * @brief Destroys the light shadow textures manager. This method releases all
     * resources associated with the manager and prepares it for deletion.
     */
    virtual void destroy() = 0;

  protected:
    ILightShadowTexturesManager();
  };
}
