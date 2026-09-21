#pragma once

#include "hc/graphics/resource/frameBuffer/hcIFrameBuffer.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"

namespace hc
{
  class ITexture;
  class IFrameBuffer;

  /**
   * Interface for geometry buffer resources used by deferred rendering.
   *
   * @note The parameter of type Color of the clear(const Color&) method inherited from
   * IFrameBuffer is not used for IGBuffer. Instead, it uses the predefined clear color
   * constants for each attachment when clearing the GBuffer.
   */
  class HC_CORE_EXPORT IGBuffer : public IFrameBuffer
  {
  public:
    static inline constexpr float CLEAR_COLOR_POSITION_AND_DEPTH[4] = { 0.0f, 0.0f, 0.0f, -1.0f };
    static inline constexpr float CLEAR_COLOR_NORMAL[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static inline constexpr float CLEAR_COLOR_ALBEDO_AND_ALPHA[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static inline constexpr float CLEAR_COLOR_ORMIOR[4] = { 1.0f, 1.0f, 0.0f, 1.5f / assets::materialDescriptor::MAXIMUM_IOR };

    virtual ~IGBuffer();

    /**
     * Binds the geometry buffer textures attachments for sampling.
     *
     * @param positionAndDepthTextureUnit The texture unit for the position and depth
     * texture.
     * @param normalTextureUnit The texture unit for the normal texture.
     * @param albedoAndAlphaTextureUnit The texture unit for the albedo and alpha texture.
     * @param ORMIORTextureUnit The texture unit for the occlusion-roughness-metallic
     * (ORM) and index of refraction (IOR) texture.
     */
    virtual void bindGTexturesForReading(
      UInt8 positionAndDepthTextureUnit,
      UInt8 normalTextureUnit,
      UInt8 albedoAndAlphaTextureUnit,
      UInt8 ORMIORTextureUnit
    ) = 0;

    /**
     * Gets the world-space position texture attachment, and the pixel depth (non-linear).
     *
     * @return Const reference to the position texture.
     */
    virtual const ITexture& getPositionAndDepth() const = 0;

    /**
     * Gets the normal and roughness texture attachment.
     *
     * @return Const reference to the normal texture.
     */
    virtual const ITexture& getNormal() const = 0;

    /**
    * Gets the albedo and alpha texture attachment.
    *
    * @return Const reference to the albedo/alpha texture.
    */
    virtual const ITexture& getAlbedoAlpha() const = 0;

    /**
     * Gets the occlusion, roughness, metallic, and index of refraction (IOR) texture
     * attachment.
     *
     * @return Const reference to the ORM/IOR texture.
     */
    virtual const ITexture& getORMIOR() const = 0;

  protected:
    IGBuffer();
  };
}
