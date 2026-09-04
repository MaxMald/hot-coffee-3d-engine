#pragma once

#include "hc/scene/light/hcALight.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  /**
   * @brief Represents a spot light with shadow-casting configuration.
   */
  class HC_CORE_EXPORT SpotLight : public ALight
  {
  public:

    /**
     * @brief Creates a spot light with default settings.
     */
    SpotLight();

    /**
     * @brief Destroys the spot light.
     */
    ~SpotLight() override;

    /**
     * @copydoc ALight::serialize
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @copydoc ALight::deserialize
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Sets the direction of the spot light.
     * 
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the direction of the spot light.
     *
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

    /**
     * @brief Sets the inner cone angle of the spot light. This is the angle between the
     * light's direction and the edge of the light cone where the light intensity starts
     * to fall off.
     *
     * @param angle The new inner cone angle.
     */
    void setInnerConeAngle(Angle angle);

    /**
     * @brief Gets the inner cone angle of the spot light. This is the angle between the
     * light's direction and the edge of the light cone where the light intensity starts
     * to fall off.
     *
     * @return The current inner cone angle.
     */
    Angle getInnerConeAngle() const;

    /**
     * @brief Sets the outer cone angle of the spot light. This is the angle between the
     * light's direction and the edge of the light cone where the light intensity falls to
     * zero.
     *
     * @param angle The new outer cone angle.
     */
    void setOuterConeAngle(Angle angle);
    
    /**
     * @brief Gets the outer cone angle of the spot light. This is the angle between the
     * light's direction and the edge of the light cone where the light intensity falls to
     * zero.
     *
     * @return The current outer cone angle.
     */
    Angle getOuterConeAngle() const;

    /**
     * @brief Sets the near plane distance for shadow projection.
     *
     * @param nearPlane The new near plane distance.
     */
    void setShadowProjectionNearPlane(float nearPlane);

    /**
     * @brief Gets the near plane distance for shadow projection.
     *
     * @return The current near plane distance.
     */
    float getShadowProjectionNearPlane() const;

    /**
     * @brief Sets the far plane distance for shadow projection.
     *
     * @param farPlane The new far plane distance.
     */
    void setShadowProjectionFarPlane(float farPlane);

    /**
     * @brief Gets the far plane distance for shadow projection.
     *
     * @return The current far plane distance.
     */
    float getShadowProjectionFarPlane() const;

    /**
     * @brief Converts the spot light's properties to a GPU-aligned data structure for use
     * in shaders.
     *
     * @return A SpotLight data structure containing the spot light's properties.
     */
    dataBlockStructure::SpotLight getDataBlockStructure() const;

    /**
     * @brief Converts the spot light's shadow properties to a GPU-aligned data structure
     * for use in shaders.
     *
     * @param transposeMatrices Whether to transpose the matrices in the data structure.
     * 
     * @return A SpotLightShadow data structure containing the spot light's shadow
     * properties.
     */
    dataBlockStructure::SpotLightShadow getShadowDataBlockStructure(
      bool transposeMatrices
    ) const;

  private:
    Vector3f m_direction;
    Angle m_innerConeAngle;
    Angle m_outerConeAngle;
    float m_shadowProjectionNearPlane;
    float m_shadowProjectionFarPlane;
  };
}
