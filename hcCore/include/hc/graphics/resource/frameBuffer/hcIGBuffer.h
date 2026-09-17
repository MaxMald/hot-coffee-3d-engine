#pragma once

#include "hc/graphics/resource/frameBuffer/hcIFrameBuffer.h"

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
    static inline constexpr float CLEAR_COLOR_NORMAL_AND_ROUGHNESS[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static inline constexpr float CLEAR_COLOR_ALBEDO_AND_ALPHA[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static inline constexpr float CLEAR_COLOR_MATERIAL_PARAMETERS[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static inline constexpr float CLEAR_COLOR_SPECULAR_COLOR_AND_SHININESS[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    virtual ~IGBuffer();

    /**
     * Binds the geometry buffer textures attachments for sampling.
     *
     * @param positionAndDepthTextureUnit The texture unit for the position and depth
     * texture.
     * @param normalAndRoughnessTextureUnit The texture unit for the normal and roughness
     * texture.
     * @param albedoAndAlphaTextureUnit The texture unit for the albedo and alpha texture.
     * @param materialParametersTextureUnit The texture unit for the material parameters
     * texture.
     * @param specularColorAndShininessTextureUnit The texture unit for the specular color
     * and shininess texture.
     */
    virtual void bindGTexturesForReading(
      UInt8 positionAndDepthTextureUnit,
      UInt8 normalAndRoughnessTextureUnit,
      UInt8 albedoAndAlphaTextureUnit,
      UInt8 materialParametersTextureUnit,
      UInt8 specularColorAndShininessTextureUnit
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
     * - x component: free
     * - y component: free
     * - z component: free
     * - w component: free
     *
     * @return Const reference to the material parameters texture.
     */
    virtual const ITexture& getMaterialParameters() const = 0;

    /**
     * Gets the specular color and shininess texture attachment.
     *
     * @return Const reference to the specular color and shininess texture.
     */
    virtual const ITexture& getSpecularColorAndShininess() const = 0;

  protected:
    IGBuffer();
  };
}
