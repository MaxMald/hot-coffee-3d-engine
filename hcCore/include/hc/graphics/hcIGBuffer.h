#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcIFrameBuffer.h"

namespace hc
{
  class ITexture;
  class IFrameBuffer;

  /**
   * Interface for geometry buffer resources used by deferred rendering.
   */
  class HC_CORE_EXPORT IGBuffer : public IFrameBuffer
  {
  public:
    virtual ~IGBuffer();

    /**
     * Binds the geometry buffer textures attachments for sampling.
     */
    virtual void bindGTexturesForReading() = 0;

    /**
     * Gets the world-space position texture attachment, and the pixel depth (non-linear).
     *
     * @return Const reference to the position texture.
     */
    virtual const ITexture& getPositionAndDepth() const = 0;

    /**
     * Gets the normal and roughness texture attachment.
     *
     * @return Const reference to the normal/roughness texture.
     */
    virtual const ITexture& getNormalRoughness() const = 0;

    /**
    * Gets the albedo and alpha texture attachment.
    *
    * @return Const reference to the albedo/alpha texture.
    */
    virtual const ITexture& getAlbedoAlpha() const = 0;

    /**
     * Gets the material parameter texture attachment.
     *
     * - x component: specular strength
     * - y component: free
     * - z component: free
     * - w component: free
     *
     * @return Const reference to the material parameters texture.
     */
    virtual const ITexture& getMaterialParameters() const = 0;

  protected:
    IGBuffer();
  };
}
