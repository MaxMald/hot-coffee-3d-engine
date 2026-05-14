#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"

namespace hc
{
  /**
   * @brief Represents a light source in the engine.
   */
  class HC_CORE_EXPORT ALight : public ISerializable
  {
  public:
    virtual ~ALight() = default;

    /**
     * @copydoc ISerializable::serialize
     */
    virtual void serialize(BinaryWriter& writer) const override;
    
    /**
     * @copydoc ISerializable::deserialize
     */
    virtual void deserialize(BinaryReader& reader) override;

    /**
     * @brief Gets the unique identifier of the light.
     * 
     * @return The UUID of the light.
     */
    const UUID& getUUID() const;

    /**
     * @brief Gets the type of the light.
     * 
     * @return The current light type.
     */
    lightType::Type getType() const;

    /**
     * @brief Sets the color of the light.
     * 
     * @param color The new color.
     */
    void setColor(const Color& color);

    /**
     * @brief Sets the color of the light using RGBA components.
     * 
     * @param r Red component.
     * @param g Green component.
     * @param b Blue component.
     */
    void setColor(float r, float g, float b);

    /**
     * @brief Gets the color of the light.
     * 
     * @return The current color.
     */
    const Color& getColor() const;

    /**
     * @brief Sets the intensity of the light.
     * 
     * @param intensity The new intensity value.
     */
    void setIntensity(float intensity);

    /**
     * @brief Gets the intensity of the light.
     * 
     * @return The current intensity value.
     */
    float getIntensity() const;

    /**
     * @brief Sets the position of the light.
     * 
     * @param position The new position vector.
     */
    void setPosition(const Vector3f& position);

    /**
     * @brief Gets the position of the light.
     * 
     * @return The current position vector.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Enables or disables the light.
     */
    void setEnabled(bool isEnabled);

    /**
     * @brief Checks if the light is enabled.
     * 
     * @return True if the light is enabled, false otherwise.
     */
    bool isEnabled() const;

  protected:
    ALight(lightType::Type type);

  private:
    UUID m_id;
    bool enabled;
    lightType::Type m_type;
    Color m_color;
    float m_intensity;
    Vector3f m_position;
  };
}
