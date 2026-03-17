#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"

namespace hc
{
  /**
   * @brief Represents a light source in the engine.
   */
  class HC_CORE_EXPORT Light : public ISerializable
  {
  public:
    /**
     * @brief Constructs a Light with the specified type.
     * 
     * @param type The type of the light (default is Point).
     */
    Light(lightType::Type type = lightType::Type::Point);

    /**
     * @copydoc ISerializable::serialize
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Gets the unique identifier of the light.
     * 
     * @return The UUID of the light.
     */
    const UUID& getUUID() const;

    /**
     * @brief Sets the type of the light.
     * 
     * @param type The new light type.
     */
    void setType(lightType::Type type);

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
     * @brief Sets the direction of the light.
     * 
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the direction of the light.
     * 
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

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

  private:
    UUID m_id;
    bool enabled;
    lightType::Type m_type;
    Color m_color;
    float m_intensity;
    Vector3f m_position;
    Vector3f m_direction;
  };
}
