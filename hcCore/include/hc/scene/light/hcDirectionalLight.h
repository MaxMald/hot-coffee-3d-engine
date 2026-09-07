#pragma once

#include "hc/scene/light/hcALight.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  /**
   * @brief Represents a directional light with shadow-casting configuration.
   */
  class HC_CORE_EXPORT DirectionalLight : public ALight
  {
  public:
    /**
     * @brief Creates a directional light with default direction and shadow settings.
     */
    DirectionalLight();

    /**
     * @brief Destroys the directional light.
     */
    ~DirectionalLight() override;

    /**
     * @copydoc ALight::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc ALight::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Sets the light direction.
     *
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the light direction.
     *
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

    /**
     * @brief Sets the point the shadow camera looks at.
     *
     * @param target The new shadow view target in world space.
     */
    void setShadowViewTarget(const Vector3f& target);

    /**
     * @brief Gets the point the shadow camera looks at.
     *
     * @return The current shadow view target in world space.
     */
    const Vector3f& getShadowViewTarget() const;

    /**
     * @brief Sets the distance between the shadow target and the shadow camera. This is
     * used to position the shadow camera along the light direction.
     *
     * @param distance The new shadow view distance.
     */
    void setShadowViewDistance(float distance);
    
    /**
     * @brief Gets the distance between the shadow target and the shadow camera.
     *
     * @return The current shadow view distance.
     */
    float getShadowViewDistance() const;

    /**
     * @brief Sets the size of the directional light orthographic shadow projection.
     *
     * @param size The new projection size.
     */
    void setShadowProjectionSize(float size);

    /**
     * @brief Gets the size of the directional light orthographic shadow projection.
     *
     * @return The current projection size.
     */
    float getShadowProjectionSize() const;

    /**
     * @brief Sets the near plane of the shadow projection.
     *
     * @param nearPlane The new near plane distance.
     */
    void setShadowProjectionNearPlane(float nearPlane);

    /**
     * @brief Gets the near plane of the shadow projection.
     *
     * @return The current near plane distance.
     */
    float getShadowProjectionNearPlane() const;

    /**
     * @brief Sets the far plane of the shadow projection.
     *
     * @param farPlane The new far plane distance.
     */
    void setShadowProjectionFarPlane(float farPlane);

    /**
     * @brief Gets the far plane of the shadow projection.
     *
     * @return The current far plane distance.
     */
    float getShadowProjectionFarPlane() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this directional light.
     *
     * @return A dataBlockStructure::DirectionalLight containing the light's data.
     */
    dataBlockStructure::DirectionalLight getDataBlockStructure() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this directional light's
     * shadow.
     *
     * @param transposeMatrices Whether to transpose matrices for GPU upload.
     *
     * @return A dataBlockStructure::DirectionalLightShadow containing the shadow's data.
     */
    dataBlockStructure::DirectionalLightShadow getShadowDataBlockStructure(
      bool transposeMatrices
    ) const;

  private:
    Vector3f m_direction;
    Vector3f m_shadowViewTarget;
    float m_shadowViewDistance;
    float m_shadowProjectionSize;
    float m_shadowProjectionNearPlane;
    float m_shadowProjectionFarPlane;
  };
}
